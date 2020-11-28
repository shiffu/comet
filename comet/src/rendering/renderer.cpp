#include <glad/glad.h>
#include <comet/renderer.h>
#include <set>
#include <unordered_map>
#include <glm/mat4x4.hpp>
#include <comet/shader.h>
#include <comet/vertexArray.h>
#include <comet/indexBuffer.h>
#include <comet/vertexBuffer.h>
#include <comet/commandBuffer.h>
#include <comet/mesh.h>
#include <comet/light.h>

namespace comet
{
    struct MultiDrawKey
    {
        std::string materialName{};
        bool hasIndices;
    
        bool operator==(const MultiDrawKey& other) const
        {
            return materialName == other.materialName && hasIndices == other.hasIndices;
        }
    };

    struct hash_fn
    {
        std::size_t operator()(const MultiDrawKey& key) const
        {
            std::size_t h1 = std::hash<std::string>()(key.materialName);
            std::size_t h2 = std::hash<bool>()(key.hasIndices);

            return h1 ^ h2;
        }
    };

    struct MultiDrawIndirectContext
    {
        MultiDrawIndirectContext(Material* _material) : material(_material) {}

        Material* material{nullptr};
        VertexArray vao{};
        IndexBuffer ibo{GL_STATIC_DRAW};
        VertexBuffer vbo{GL_STATIC_DRAW};
        VertexBuffer instanceBuffer{GL_DYNAMIC_DRAW};
        CommandBuffer commandBuffer{GL_DYNAMIC_DRAW};
        std::vector<Mesh*> meshes;
    };

    struct ShaderDrawContext
    {
        ShaderDrawContext() {}

        Shader* shader{nullptr};
        std::unordered_map<MultiDrawKey, MultiDrawIndirectContext*, hash_fn> multiDrawIndirectContexts;
    };

    Renderer::~Renderer()
    {
        for (auto [shaderName, pShaderDrawContext] : m_shaderDrawContexts)
        {
            for (auto [key, pMaterialDrawContext] : pShaderDrawContext->multiDrawIndirectContexts)
            {
                delete pMaterialDrawContext;
                pMaterialDrawContext = nullptr;
            }

            delete pShaderDrawContext;
        }
    }

    void Renderer::addLight(Light* light)
    {
        m_lights.push_back(light);
    }

    void Renderer::addMesh(Mesh* mesh)
    {
        std::set<MultiDrawIndirectContext*> multiDrawIndirectContextsToUpdate{};
        auto meshMaterial = mesh->getMeshMaterial();
        
        for (auto& meshInstance : mesh->getMeshInstances())
        {
            // Default to the mesh Material which is always there if the mesh instance
            // doesn't have any material attached to it
            auto meshInstanceMaterial = meshInstance.getMaterial();
            if (meshInstanceMaterial == nullptr)
            {
                if (meshMaterial == nullptr)
                {
                    meshMaterial = &m_defaultColorMaterial;
                }
                meshInstanceMaterial = meshMaterial;
                meshInstance.setMaterial(meshInstanceMaterial);
            }
            auto materialName = meshInstanceMaterial->getName();
            auto shaderName = meshInstanceMaterial->getShaderName();

            // Check if a new Shader Draw context is needed
            if (m_shaderDrawContexts.find(shaderName) == m_shaderDrawContexts.end())
            {
                m_shaderDrawContexts[shaderName] = new ShaderDrawContext();
                m_shaderDrawContexts[shaderName]->shader = meshInstanceMaterial->getShader();
            }

            // Check if a new Material Indirect Draw context is needed
            auto key = MultiDrawKey{materialName, mesh->hasIndices()};
            auto& drawContextMap = m_shaderDrawContexts[shaderName]->multiDrawIndirectContexts;
            if (drawContextMap.find(key) == drawContextMap.end())
            {
                drawContextMap[key] = new MultiDrawIndirectContext(meshInstanceMaterial);
            }
            multiDrawIndirectContextsToUpdate.insert(drawContextMap[key]);
        }

        // Update Context Buffers
        for (auto currentDrawContext : multiDrawIndirectContextsToUpdate)
        {
            if (mesh->hasIndices())
            {
                currentDrawContext->ibo.increaseSize(mesh->getIndicesSize());
            }

            currentDrawContext->commandBuffer.increaseSize(sizeof(DrawElementsIndirectCommand));
            currentDrawContext->vbo.increaseSize(mesh->getVerticesSize());
            currentDrawContext->instanceBuffer.increaseSize(mesh->getInstanceCount() * mesh->getInstanceDataSize());
            currentDrawContext->meshes.push_back(mesh);
        }
    }

    void Renderer::allocateBuffersAndSetupLayouts()
    {
        for (auto [shaderName, pShaderDrawContext] : m_shaderDrawContexts)
        {
            for (auto [key, pMaterialDrawContext] : pShaderDrawContext->multiDrawIndirectContexts)
            {
                // Buffers Allocation
                pMaterialDrawContext->vbo.allocate();
                pMaterialDrawContext->instanceBuffer.allocate();
                pMaterialDrawContext->commandBuffer.allocate();

                // Buffers Layouts definition
                VertexBufferLayout vboLayout;
                // Assuming that all mesh in a renderer have the same data structure
                // TODO: Should we consider having multiple Vertex data structure within on renderer?
                auto firstMesh = pMaterialDrawContext->meshes[0];
                firstMesh->updateVboDataLayout(vboLayout);

                if (pMaterialDrawContext->ibo.getSize())
                {
                    pMaterialDrawContext->ibo.allocate();
                    pMaterialDrawContext->vao.addLayout(vboLayout, pMaterialDrawContext->vbo, &pMaterialDrawContext->ibo);
                }
                else
                {
                    pMaterialDrawContext->vao.addLayout(vboLayout, pMaterialDrawContext->vbo, nullptr);
                }

                VertexBufferLayout instanceDataLayout;
                firstMesh->updateInstanceDataLayout(instanceDataLayout);
                pMaterialDrawContext->vao.addLayout(instanceDataLayout, pMaterialDrawContext->instanceBuffer);
            }
        }
    }

    void Renderer::loadData()
    {
        for (auto [shaderName, pShaderDrawContext] : m_shaderDrawContexts)
        {
            for (auto [key, pMaterialDrawContext] : pShaderDrawContext->multiDrawIndirectContexts)
            {
                uint32_t firstIndex{0};
                uint32_t firstVertex{0};
                uint32_t baseVertex{0};

                // Map Buffers memory
                pMaterialDrawContext->vbo.mapMemory(GL_WRITE_ONLY);
                pMaterialDrawContext->instanceBuffer.mapMemory(GL_WRITE_ONLY);
                pMaterialDrawContext->commandBuffer.mapMemory(GL_WRITE_ONLY);
                bool indexBufferExists = (pMaterialDrawContext->ibo.getSize() > 0);

                if (indexBufferExists)
                {
                    pMaterialDrawContext->ibo.mapMemory(GL_WRITE_ONLY);
                }

                // Load Data from Meshes and Mesh Instances
                for (auto mesh : pMaterialDrawContext->meshes)
                {
                    uint32_t indexCount = mesh->getIndexCount();
                    if (indexCount > 0)
                    {
                        firstIndex = pMaterialDrawContext->ibo.getCurrentCount();
                        auto indices = mesh->getIndices();
                        pMaterialDrawContext->ibo.loadDataInMappedMemory(indices.data(), indexCount);
                    }

                    auto vertices = mesh->getVertices();
                    auto verticesSize = mesh->getVerticesSize();
                    auto vertexCount = mesh->getVertexCount();
                    firstVertex = pMaterialDrawContext->vbo.getCurrentCount();
                    pMaterialDrawContext->vbo.loadDataInMappedMemory((const void*)vertices.data(), verticesSize, vertexCount);

                    auto instanceCount =  mesh->getInstanceCount();
                    auto baseInstance =  pMaterialDrawContext->instanceBuffer.getCurrentCount();

                    for (auto instance : mesh->getMeshInstances())
                    {
                        auto data = instance.getInstanceData();
                        pMaterialDrawContext->instanceBuffer.loadDataInMappedMemory((const void*)&data, sizeof(data), 1);
                    }

                    if (indexCount > 1)
                    {
                        DrawElementsIndirectCommand cmd{indexCount, instanceCount, firstIndex, baseVertex, baseInstance};
                        pMaterialDrawContext->commandBuffer.loadDataInMappedMemory((const void*)&cmd, sizeof(cmd), 1);
                        baseVertex += vertexCount;
                    }
                    else
                    {
                        DrawArraysIndirectCommand cmd{vertexCount, instanceCount, firstVertex, baseInstance};
                        pMaterialDrawContext->commandBuffer.loadDataInMappedMemory((const void*)&cmd, sizeof(cmd), 1);
                    }
                }

                pMaterialDrawContext->ibo.unmapMemory();
                pMaterialDrawContext->vbo.unmapMemory();
                pMaterialDrawContext->instanceBuffer.unmapMemory();
                pMaterialDrawContext->commandBuffer.unmapMemory();
            }
        }
    }

    void Renderer::reloadInstanceData()
    {
        for (auto [shaderName, pShaderDrawContext] : m_shaderDrawContexts)
        {
            for (auto [key, pMaterialDrawContext] : pShaderDrawContext->multiDrawIndirectContexts)
            {
                pMaterialDrawContext->instanceBuffer.mapMemory(GL_WRITE_ONLY);
                for (auto mesh : pMaterialDrawContext->meshes)
                {
                    for (auto instance : mesh->getMeshInstances())
                    {
                        auto data = instance.getInstanceData();
                        pMaterialDrawContext->instanceBuffer.loadDataInMappedMemory((const void*)&data, sizeof(data), 1);
                    }
                }
                pMaterialDrawContext->instanceBuffer.unmapMemory();
            }
        }
    }

    void Renderer::render(const Camera& camera)
    {
        auto vpMatrix = camera.getViewProjection();
        auto viewMatrix = camera.getView();
        auto projectionMatrix = camera.getProjection();

        for (auto [shaderName, pShaderDrawContext] : m_shaderDrawContexts)
        {
            auto currentShader = pShaderDrawContext->shader;
            currentShader->bind();
            currentShader->setUniform(currentShader->getViewProjectionMatrixName(), vpMatrix);
            currentShader->setUniform(currentShader->getViewMatrixName(), viewMatrix);
            currentShader->setUniform(currentShader->getProjectionMatrixName(), projectionMatrix);

            for (auto light : getLights())
            {
                light->loadUniforms(currentShader);
            }

            for (auto [key, pMaterialDrawContext] : pShaderDrawContext->multiDrawIndirectContexts)
            {
                auto currentMaterial = pMaterialDrawContext->material;
                currentMaterial->loadUniforms();

                pMaterialDrawContext->vao.bind();
                pMaterialDrawContext->commandBuffer.bind();

                if (key.hasIndices)
                {
                    glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, nullptr,
                                    pMaterialDrawContext->commandBuffer.getCurrentCount(), 0);
                }
                else
                {
                    glMultiDrawArraysIndirect(GL_TRIANGLES, nullptr, 1, 0);
                }
            }
        }
    }

} // namespace comet