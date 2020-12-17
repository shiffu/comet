#include <glad/glad.h>
#include <comet/renderer.h>
#include <glm/mat4x4.hpp>
#include <comet/shader.h>
#include <comet/vertexArray.h>
#include <comet/indexBuffer.h>
#include <comet/vertexBuffer.h>
#include <comet/commandBuffer.h>
#include <comet/light.h>
#include <comet/utils.h>
#include <comet/scene.h>
#include <comet/components.h>
#include <comet/materialRegistry.h>
#include <comet/resourceManager.h>


namespace comet
{
    struct MultiDrawKey
    {
        uint32_t materialType{};
        bool hasIndices;
    
        bool operator==(const MultiDrawKey& other) const
        {
            return materialType == other.materialType && hasIndices == other.hasIndices;
        }
    };

    struct hash_fn
    {
        std::size_t operator()(const MultiDrawKey& key) const
        {
            std::size_t h2 = std::hash<bool>()(key.hasIndices);

            return key.materialType ^ h2;
        }
    };

    struct MeshInstanceData
    {
        MeshInstanceData() = default;
        MeshInstanceData(const glm::mat4& transform, uint32_t materialInstanceId)
            : modelTransform(transform), materialInstanceId(materialInstanceId) {}

        glm::mat4 modelTransform{1.0f};
        uint32_t materialInstanceId{0};
    };

    struct MeshAndInstances
    {
        StaticMesh* staticMesh;
        std::vector<MeshInstanceData> instancesData;
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

        std::unordered_map<uint32_t, MeshAndInstances> staticMeshes;
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

    void Renderer::addScene(Scene* scene)
    {
        m_scene = scene;
        auto& registry = m_scene->m_registry;

        // TODO: From the doc: consider creating the group when no components have been assigned yet.
        // If the registry is empty, preparation is extremely fast.
        registry.group<TransformComponent, MeshComponent>().each([&](auto& transform, auto& mesh)
        {
            auto material = MaterialRegistry::getInstance().getMaterial(mesh.materialTypeId , mesh.materialInstanceId);
            if (material == nullptr)
            {
                material = &m_defaultColorMaterial;
            }

            // Check if a new Shader Draw context is needed
            auto shader = material->getShader();
            auto shaderTypeHash = shader->getTypeHash();
            if (m_shaderDrawContexts.find(shaderTypeHash) == m_shaderDrawContexts.end())
            {
                m_shaderDrawContexts[shaderTypeHash] = new ShaderDrawContext();
                m_shaderDrawContexts[shaderTypeHash]->shader = shader;
            }

            // Check if a new Material Indirect Draw context is needed
            auto staticMeshHandler = ResourceManager::getInstance().getStaticMesh(mesh.meshTypeId);
            auto staticMesh = staticMeshHandler.resource;
            auto hasIndices = staticMesh->hasIndices();
            auto key = MultiDrawKey{mesh.materialTypeId, hasIndices};
            auto& drawContextMap = m_shaderDrawContexts[shaderTypeHash]->multiDrawIndirectContexts;
            if (drawContextMap.find(key) == drawContextMap.end())
            {
                drawContextMap[key] = new MultiDrawIndirectContext(material);
            }

            auto currentDrawContext = drawContextMap[key];
            // Check if we are dealing with a new StaticMesh (to update buffers' size)
            if (currentDrawContext->staticMeshes.find(staticMeshHandler.resourceId) == currentDrawContext->staticMeshes.end())
            {
                if (hasIndices)
                {
                    currentDrawContext->ibo.increaseSize(staticMesh->getIndicesSize());
                    currentDrawContext->commandBuffer.increaseSize(sizeof(DrawElementsIndirectCommand));
                }
                else
                {
                    currentDrawContext->commandBuffer.increaseSize(sizeof(DrawArraysIndirectCommand));
                }

                currentDrawContext->vbo.increaseSize(staticMesh->getVerticesSize());
            }

            currentDrawContext->instanceBuffer.increaseSize(sizeof(MeshInstanceData));
            auto& meshAndInstances = currentDrawContext->staticMeshes[staticMeshHandler.resourceId];
            meshAndInstances.staticMesh = staticMesh;
            meshAndInstances.instancesData.emplace_back(transform.transform, mesh.materialInstanceId);
        });
    }

    void Renderer::allocateBuffersAndSetupLayouts()
    {
        for (auto [shaderType, pShaderDrawContext] : m_shaderDrawContexts)
        {
            for (auto [key, pMaterialDrawContext] : pShaderDrawContext->multiDrawIndirectContexts)
            {
                // Buffers Allocation
                pMaterialDrawContext->vbo.allocate();
                pMaterialDrawContext->instanceBuffer.allocate();
                pMaterialDrawContext->commandBuffer.allocate();

                // Buffers Layouts definition
                VertexBufferLayout vboLayout;
                pMaterialDrawContext->material->updateVboDataLayout(vboLayout);

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
                pMaterialDrawContext->material->updateInstanceDataLayout(instanceDataLayout);
                pMaterialDrawContext->vao.addLayout(instanceDataLayout, pMaterialDrawContext->instanceBuffer);
            }
        }
    }

    void Renderer::loadData()
    {
        for (auto [shaderType, pShaderDrawContext] : m_shaderDrawContexts)
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
                for (auto [staticMeshId, meshAndInstancesData] : pMaterialDrawContext->staticMeshes)
                {
                    auto staticMesh = meshAndInstancesData.staticMesh;
                    uint32_t indexCount = staticMesh->getIndexCount();
                    if (indexCount > 0)
                    {
                        firstIndex = pMaterialDrawContext->ibo.getCurrentCount();
                        auto indices = staticMesh->getIndices();
                        pMaterialDrawContext->ibo.loadDataInMappedMemory(indices.data(), indexCount);
                    }

                    auto vertices = staticMesh->getVertices();
                    auto verticesSize = staticMesh->getVerticesSize();
                    auto vertexCount = staticMesh->getVertexCount();
                    firstVertex = pMaterialDrawContext->vbo.getCurrentCount();
                    pMaterialDrawContext->vbo.loadDataInMappedMemory((const void*)vertices.data(), verticesSize, vertexCount);

                    auto instanceCount =  static_cast<uint32_t>(meshAndInstancesData.instancesData.size());
                    auto baseInstance =  pMaterialDrawContext->instanceBuffer.getCurrentCount();

                    for (auto& instanceData : meshAndInstancesData.instancesData)
                    {
                        pMaterialDrawContext->instanceBuffer.loadDataInMappedMemory((const void*)&instanceData, sizeof(instanceData), 1);
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
        auto& registry = m_scene->m_registry;
        auto currentMeshId{0};
        auto currentMaterialId{-1};
        uint32_t currentMeshTypeId{0};
        uint32_t currentMaterialTypeId{0};
        
        registry.group<TransformComponent, MeshComponent>().each([&](auto& transform, auto& mesh)
        {
            ResourceHandler<StaticMesh> staticMeshHandler;
            StaticMesh* staticMesh;
            bool hasIndices;

            if (currentMeshTypeId != mesh.meshTypeId)
            {
                staticMeshHandler = ResourceManager::getInstance().getStaticMesh(mesh.meshTypeId);
                staticMesh = staticMeshHandler.resource;
                hasIndices = staticMesh->hasIndices();
            }

            MultiDrawKey key;
            Material* material{nullptr};
            if (currentMaterialTypeId != mesh.materialTypeId)
            {
                key = MultiDrawKey{mesh.materialTypeId, hasIndices};
                material = MaterialRegistry::getInstance().getMaterial(mesh.materialTypeId , mesh.materialInstanceId);
            }

            auto shaderTypeHash = material->getShader()->getTypeHash();
            auto& drawContextMap = m_shaderDrawContexts[shaderTypeHash]->multiDrawIndirectContexts;
            auto currentDrawContext = drawContextMap[key];           
            auto& meshAndInstances = currentDrawContext->staticMeshes[staticMeshHandler.resourceId];
            meshAndInstances.staticMesh = staticMesh;
            if (currentMeshId != staticMeshHandler.resourceId)
            {
                currentMeshId = staticMeshHandler.resourceId;
                meshAndInstances.instancesData.clear();
            }

            meshAndInstances.instancesData.emplace_back(transform.transform, mesh.materialInstanceId);
        });

        for (auto [shaderType, pShaderDrawContext] : m_shaderDrawContexts)
        {
            for (auto [key, pMaterialDrawContext] : pShaderDrawContext->multiDrawIndirectContexts)
            {
                // TODO: This mapMemory can be quite slow for big buffers. Check if splitting in multiple buffers would be faster!
                pMaterialDrawContext->instanceBuffer.mapMemory(GL_WRITE_ONLY);
                for (auto [staticMeshId, meshAndInstancesData] : pMaterialDrawContext->staticMeshes)
                {
                    for (auto& instanceData : meshAndInstancesData.instancesData)
                    {
                        pMaterialDrawContext->instanceBuffer.loadDataInMappedMemory((const void*)&instanceData, sizeof(instanceData), 1);
                    }
                }
                pMaterialDrawContext->instanceBuffer.unmapMemory();
            }
        }
    }

    void Renderer::render(const glm::mat4& view, const glm::mat4& projection)
    {
        auto vpMatrix = projection * view;

        for (auto [shaderType, pShaderDrawContext] : m_shaderDrawContexts)
        {
            auto currentShader = pShaderDrawContext->shader;
            currentShader->bind();
            currentShader->setUniform(currentShader->getViewProjectionMatrixName(), vpMatrix);
            currentShader->setUniform(currentShader->getViewMatrixName(), view);
            currentShader->setUniform(currentShader->getProjectionMatrixName(), projection);

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
                    glMultiDrawArraysIndirect(GL_TRIANGLES, nullptr,
                                    pMaterialDrawContext->commandBuffer.getCurrentCount(), 0);
                }
            }
        }
    }

} // namespace comet