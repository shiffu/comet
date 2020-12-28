#include <glad/glad.h>

#include <comet/renderer.h>
#include <comet/shader.h>
#include <rendering/vertexArray.h>
#include <rendering/indexBuffer.h>
#include <rendering/vertexBuffer.h>
#include <rendering/commandBuffer.h>
#include <comet/light.h>
#include <comet/utils.h>
#include <comet/scene.h>
#include <comet/components.h>
#include <comet/materialRegistry.h>
#include <comet/resourceManager.h>

#include <glm/mat4x4.hpp>
#include <unordered_set>

#include <comet/log.h>

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
        std::unique_ptr<VertexArray> vao;
        std::unique_ptr<IndexBuffer> ibo;
        std::unique_ptr<VertexBuffer> vbo;
        std::unique_ptr<VertexBuffer> instanceBuffer;
        std::unique_ptr<CommandBuffer> commandBuffer;

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
        cleanUp();
    }

    void Renderer::cleanUp()
    {
        for (auto [shaderName, pShaderDrawContext] : m_shaderDrawContexts)
        {
            for (auto [key, pMaterialDrawContext] : pShaderDrawContext->multiDrawIndirectContexts)
            {
                delete pMaterialDrawContext;
                pMaterialDrawContext = nullptr;
            }
            pShaderDrawContext->multiDrawIndirectContexts.clear();

            delete pShaderDrawContext;
        }
        m_shaderDrawContexts.clear();
    }

    void Renderer::addLight(Light* light)
    {
        m_lights.push_back(light);
    }

    void Renderer::setScene(Scene* scene)
    {
        m_isReady = false;
        cleanUp();
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
                auto context = new MultiDrawIndirectContext(material);
                context->ibo = IndexBuffer::create(GL_STATIC_DRAW);
                context->vbo = VertexBuffer::create(GL_STATIC_DRAW);
                context->instanceBuffer = VertexBuffer::create(GL_DYNAMIC_DRAW);
                context->commandBuffer = CommandBuffer::create(GL_DYNAMIC_DRAW);
                context->vao = VertexArray::create();

                drawContextMap[key] = context;
            }

            auto currentDrawContext = drawContextMap[key];
            // Check if we are dealing with a new StaticMesh (to update buffers' size)
            if (currentDrawContext->staticMeshes.find(staticMeshHandler.resourceId) == currentDrawContext->staticMeshes.end())
            {
                if (hasIndices)
                {
                    currentDrawContext->ibo->increaseSize(staticMesh->getIndicesSize());
                    currentDrawContext->commandBuffer->increaseSize(sizeof(DrawElementsIndirectCommand));
                }
                else
                {
                    currentDrawContext->commandBuffer->increaseSize(sizeof(DrawArraysIndirectCommand));
                }

                currentDrawContext->vbo->increaseSize(staticMesh->getVerticesSize());
            }

            currentDrawContext->instanceBuffer->increaseSize(sizeof(MeshInstanceData));
            auto& meshAndInstances = currentDrawContext->staticMeshes[staticMeshHandler.resourceId];
            meshAndInstances.staticMesh = staticMesh;
            meshAndInstances.instancesData.emplace_back(transform.getTransform(), mesh.materialInstanceId);
        });
    }

    void Renderer::allocateBuffersAndSetupLayouts()
    {
        for (auto [shaderType, pShaderDrawContext] : m_shaderDrawContexts)
        {
            for (auto [key, pMaterialDrawContext] : pShaderDrawContext->multiDrawIndirectContexts)
            {
                // Buffers Allocation
                pMaterialDrawContext->vbo->allocate();
                pMaterialDrawContext->instanceBuffer->allocate();
                pMaterialDrawContext->commandBuffer->allocate();

                // Buffers Layouts definition
                auto vboLayoutPtr = VertexBufferLayout::create();
                auto& vboLayout = *vboLayoutPtr.get();
                pMaterialDrawContext->material->updateVboDataLayout(vboLayout);

                if (pMaterialDrawContext->ibo->getSize())
                {
                    pMaterialDrawContext->ibo->allocate();
                    pMaterialDrawContext->vao->addLayout(vboLayout, *pMaterialDrawContext->vbo.get(), pMaterialDrawContext->ibo.get());
                }
                else
                {
                    pMaterialDrawContext->vao->addLayout(vboLayout, *pMaterialDrawContext->vbo.get(), nullptr);
                }

                auto instanceDataLayoutPtr = VertexBufferLayout::create();
                auto& instanceDataLayout = *instanceDataLayoutPtr.get();
                pMaterialDrawContext->material->updateInstanceDataLayout(instanceDataLayout);
                pMaterialDrawContext->vao->addLayout(instanceDataLayout, *pMaterialDrawContext->instanceBuffer.get());
            }
        }
    }

    void Renderer::loadData()
    {
        auto& sceneStats = m_scene->getStatistics();
        sceneStats.indicesCount = 0;
        sceneStats.verticesCount = 0;
        sceneStats.drawCommandsCount = 0;

        for (auto [shaderType, pShaderDrawContext] : m_shaderDrawContexts)
        {
            for (auto [key, pMaterialDrawContext] : pShaderDrawContext->multiDrawIndirectContexts)
            {
                uint32_t firstIndex{0};
                uint32_t firstVertex{0};
                uint32_t baseVertex{0};

                // Map Buffers memory
                pMaterialDrawContext->vbo->mapMemory(GL_WRITE_ONLY);
                pMaterialDrawContext->instanceBuffer->mapMemory(GL_WRITE_ONLY);
                pMaterialDrawContext->commandBuffer->mapMemory(GL_WRITE_ONLY);
                bool indexBufferExists = (pMaterialDrawContext->ibo->getSize() > 0);

                if (indexBufferExists)
                {
                    pMaterialDrawContext->ibo->mapMemory(GL_WRITE_ONLY);
                }

                // Load Data from Meshes and Mesh Instances
                for (auto [staticMeshId, meshAndInstancesData] : pMaterialDrawContext->staticMeshes)
                {
                    auto staticMesh = meshAndInstancesData.staticMesh;
                    uint32_t indexCount = staticMesh->getIndexCount();
                    if (indexCount > 0)
                    {
                        firstIndex = pMaterialDrawContext->ibo->getCount();
                        auto indices = staticMesh->getIndices();
                        pMaterialDrawContext->ibo->loadDataInMappedMemory(indices.data(), indexCount);
                    }

                    auto vertices = staticMesh->getVertices();
                    auto verticesSize = staticMesh->getVerticesSize();
                    auto vertexCount = staticMesh->getVertexCount();
                    firstVertex = pMaterialDrawContext->vbo->getCount();
                    pMaterialDrawContext->vbo->loadDataInMappedMemory((const void*)vertices.data(), verticesSize, vertexCount);

                    auto instanceCount =  static_cast<uint32_t>(meshAndInstancesData.instancesData.size());
                    auto baseInstance =  pMaterialDrawContext->instanceBuffer->getCount();

                    for (auto& instanceData : meshAndInstancesData.instancesData)
                    {
                        pMaterialDrawContext->instanceBuffer->loadDataInMappedMemory((const void*)&instanceData, sizeof(instanceData), 1);
                    }

                    if (indexCount > 1)
                    {
                        DrawElementsIndirectCommand cmd{indexCount, instanceCount, firstIndex, baseVertex, baseInstance};
                        pMaterialDrawContext->commandBuffer->loadDataInMappedMemory((const void*)&cmd, sizeof(cmd), 1);
                        baseVertex += vertexCount;
                    }
                    else
                    {
                        DrawArraysIndirectCommand cmd{vertexCount, instanceCount, firstVertex, baseInstance};
                        pMaterialDrawContext->commandBuffer->loadDataInMappedMemory((const void*)&cmd, sizeof(cmd), 1);
                    }
                }

                // Stats
                sceneStats.indicesCount = pMaterialDrawContext->ibo->getCount();
                sceneStats.verticesCount = pMaterialDrawContext->vbo->getCount();
                sceneStats.drawCommandsCount = pMaterialDrawContext->commandBuffer->getCount();

                pMaterialDrawContext->ibo->unmapMemory();
                pMaterialDrawContext->vbo->unmapMemory();
                pMaterialDrawContext->instanceBuffer->unmapMemory();
                pMaterialDrawContext->commandBuffer->unmapMemory();
            }
        }
        m_isReady = true;
    }

    void Renderer::reloadInstanceData()
    {
        T3.resume();
        if (!isReady())
            return;

        auto& sceneStats = m_scene->getStatistics();
        sceneStats.entitiesCount = 0;

        auto& registry = m_scene->m_registry;
        auto currentMaterialId{-1};
        uint32_t currentMeshTypeId{0};
        uint32_t currentMaterialTypeId{0};
        std::unordered_set<uint32_t> tmpMeshTypes;
        
        T1.resume();
        registry.group<TransformComponent, MeshComponent>().each([&](auto& transform, auto& mesh)
        {
            ResourceHandler<StaticMesh> staticMeshHandler;
            StaticMesh* staticMesh;
            bool hasIndices;

            sceneStats.entitiesCount++;

            staticMeshHandler = ResourceManager::getInstance().getStaticMesh(mesh.meshTypeId);
            staticMesh = staticMeshHandler.resource;
            hasIndices = staticMesh->hasIndices();

            MultiDrawKey key;
            Material* material{nullptr};
            key = MultiDrawKey{mesh.materialTypeId, hasIndices};
            material = MaterialRegistry::getInstance().getMaterial(mesh.materialTypeId , mesh.materialInstanceId);

            auto shaderTypeHash = material->getShader()->getTypeHash();
            auto& drawContextMap = m_shaderDrawContexts[shaderTypeHash]->multiDrawIndirectContexts;
            auto currentDrawContext = drawContextMap[key];           
            auto& meshAndInstances = currentDrawContext->staticMeshes[staticMeshHandler.resourceId];
            meshAndInstances.staticMesh = staticMesh;

            // Reset the instancesData vector first time it is accessed for a specific Mesh Type
            if (tmpMeshTypes.find(staticMeshHandler.resourceId) == tmpMeshTypes.end())
            {
                tmpMeshTypes.insert(staticMeshHandler.resourceId);
                meshAndInstances.instancesData.clear();
            }
            meshAndInstances.instancesData.emplace_back(transform.getTransform(), mesh.materialInstanceId);
        });
        T1.pause();

        T2.resume();
        for (auto [shaderType, pShaderDrawContext] : m_shaderDrawContexts)
        {
            for (auto [key, pMaterialDrawContext] : pShaderDrawContext->multiDrawIndirectContexts)
            {
                // TODO: This mapMemory can be quite slow for big buffers. Check if splitting in multiple buffers would be faster!
                pMaterialDrawContext->instanceBuffer->mapMemory(GL_WRITE_ONLY);
                for (auto& [staticMeshId, meshAndInstancesData] : pMaterialDrawContext->staticMeshes)
                {
                    for (auto& instanceData : meshAndInstancesData.instancesData)
                    {
                        pMaterialDrawContext->instanceBuffer->loadDataInMappedMemory((const void*)&instanceData, sizeof(instanceData), 1);
                    }
                }
                pMaterialDrawContext->instanceBuffer->unmapMemory();
            }
        }
        T2.pause();
        T3.pause();
    }

    void Renderer::render(const glm::mat4& view, const glm::mat4& projection)
    {
        auto& sceneStats = m_scene->getStatistics();
        sceneStats.drawCalls = 0;
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

                pMaterialDrawContext->vao->bind();
                pMaterialDrawContext->commandBuffer->bind();

                sceneStats.drawCalls++;

                if (key.hasIndices)
                {
                    glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, nullptr,
                                    pMaterialDrawContext->commandBuffer->getCount(), 0);
                }
                else
                {
                    glMultiDrawArraysIndirect(GL_TRIANGLES, nullptr,
                                    pMaterialDrawContext->commandBuffer->getCount(), 0);
                }
            }
        }
    }

} // namespace comet