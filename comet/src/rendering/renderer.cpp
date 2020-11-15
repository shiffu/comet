#include <glad/glad.h>
#include <comet/renderer.h>
#include <set>
#include <glm/mat4x4.hpp>
#include <comet/shader.h>
#include <comet/vertexArray.h>
#include <comet/indexBuffer.h>
#include <comet/vertexBuffer.h>
#include <comet/commandBuffer.h>

namespace comet
{
    struct MaterialDrawContext
    {
        MaterialDrawContext(Material* _material) : material(_material) {}

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
        Shader* shader{nullptr};
        std::unordered_map<std::string, MaterialDrawContext*> materialDrawContexts;
    };

    Renderer::~Renderer()
    {
        for (auto [shaderName, pShaderDrawContext] : m_materialDrawContexts)
        {
            for (auto [materialName, pMaterialDrawContext] : pShaderDrawContext->materialDrawContexts)
            {
                delete pMaterialDrawContext;
                pMaterialDrawContext = nullptr;
            }

            delete pShaderDrawContext;
        }
    }

    void Renderer::addMesh(Mesh* mesh)
    {
        std::set<MaterialDrawContext*> materialDrawContextsToUpdate{};
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

            if (m_materialDrawContexts.find(shaderName) == m_materialDrawContexts.end())
            {
                m_materialDrawContexts[shaderName] = new ShaderDrawContext();
                m_materialDrawContexts[shaderName]->shader = meshInstanceMaterial->getShader();
            }

            if (m_materialDrawContexts[shaderName]->materialDrawContexts.find(materialName) == m_materialDrawContexts[shaderName]->materialDrawContexts.end())
            {
                m_materialDrawContexts[shaderName]->materialDrawContexts[materialName] = new MaterialDrawContext(meshInstanceMaterial);
            }

            materialDrawContextsToUpdate.insert(m_materialDrawContexts[shaderName]->materialDrawContexts[materialName]);
        }

        for (auto currentMaterialDrawContext : materialDrawContextsToUpdate)
        {
            if (mesh->getIndexCount())
            {
                currentMaterialDrawContext->ibo.increaseSize(mesh->getIndicesSize());
                currentMaterialDrawContext->commandBuffer.increaseSize(sizeof(DrawElementsIndirectCommand));
            }
            else
            {
                currentMaterialDrawContext->commandBuffer.increaseSize(sizeof(DrawArraysIndirectCommand));
            }

            currentMaterialDrawContext->vbo.increaseSize(mesh->getVerticesSize());
            currentMaterialDrawContext->instanceBuffer.increaseSize(mesh->getInstanceCount() * mesh->getInstanceDataSize());
            currentMaterialDrawContext->meshes.push_back(mesh);
        }
    }

    void Renderer::allocateBuffersAndSetupLayouts()
    {
        for (auto [shaderName, pShaderDrawContext] : m_materialDrawContexts)
        {
            for (auto [materialName, pMaterialDrawContext] : pShaderDrawContext->materialDrawContexts)
            {
                // Buffers Allocation
                pMaterialDrawContext->vbo.allocate();
                pMaterialDrawContext->instanceBuffer.allocate();
                pMaterialDrawContext->commandBuffer.allocate();

                // Buffers Layouts definition
                // TODO: this layout setup should probably come from the Mesh class
                VertexBufferLayout vboLayout;
                vboLayout.add<float>(3, false, 0);

                if (pMaterialDrawContext->ibo.getSize())
                {
                    pMaterialDrawContext->ibo.allocate();
                    pMaterialDrawContext->vao.addLayout(vboLayout, pMaterialDrawContext->vbo, &pMaterialDrawContext->ibo);
                }
                else
                {
                    pMaterialDrawContext->vao.addLayout(vboLayout, pMaterialDrawContext->vbo, nullptr);
                }

                // TODO: this layout setup should probably come from the MeshInstance class
                VertexBufferLayout instanceDataLayout;
                instanceDataLayout.add<float>(4, false, 1, 1);
                instanceDataLayout.add<float>(4, false, 2, 1);
                instanceDataLayout.add<float>(4, false, 3, 1);
                instanceDataLayout.add<float>(4, false, 4, 1);
                instanceDataLayout.add<unsigned int>(1, false, 5, 1);
                pMaterialDrawContext->vao.addLayout(instanceDataLayout, pMaterialDrawContext->instanceBuffer);
            }
        }
    }

    void Renderer::loadData()
    {
        for (auto [shaderName, pShaderDrawContext] : m_materialDrawContexts)
        {
            for (auto [materialName, pMaterialDrawContext] : pShaderDrawContext->materialDrawContexts)
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

    void Renderer::render(const Camera& camera)
    {
        auto vpMatrix = camera.getViewProjection();
        for (auto [shaderName, pShaderDrawContext] : m_materialDrawContexts)
        {
            auto currentShader = pShaderDrawContext->shader;
            currentShader->bind();
            currentShader->setUniform(currentShader->getViewProjectionMatrixName(), vpMatrix);

            for (auto [materialName, pMaterialDrawContext] : pShaderDrawContext->materialDrawContexts)
            {
                auto currentMaterial = pMaterialDrawContext->material;
                currentMaterial->loadUniforms();

                pMaterialDrawContext->vao.bind();
                pMaterialDrawContext->commandBuffer.bind();

                for (auto mesh : pMaterialDrawContext->meshes)
                {
                    uint32_t indexCount = mesh->getIndexCount();
                    if (indexCount > 1)
                    {
                        glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, nullptr, 1, 0);
                    }
                    else
                    {
                        glMultiDrawArraysIndirect(GL_TRIANGLES, nullptr, 1, 0);
                    }
                }
            }
        }
    }

} // namespace comet