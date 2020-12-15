#include <comet/resourceManager.h>
#include <comet/log.h>
#include <core/objLoader.h>
#include <comet/utils.h>

namespace comet
{
    namespace fs = std::experimental::filesystem;
    
    fs::path ResourceManager::getResourcePath(ResourceType type) const
    {
        fs::path path = m_rootPath;

        switch(type)
        {
            case ResourceType::SHADER:
                path.append(SHADER_SUBFOLDER);
                break;

            case ResourceType::TEXTURE:
                path.append(TEXTURE_SUBFOLDER);
                break;

            case ResourceType::MESH:
                path.append(MESH_SUBFOLDER);
                break;

            case ResourceType::AUDIO:
                path.append(AUDIO_SUBFOLDER);
                break;

            default:
                // Should never reach this point
                CM_CORE_LOG_ERROR("Unmanaged resource type");
                break;
        }

        return path;
    }

    fs::path ResourceManager::getResourcePath(ResourceType type, const char* filename) const
    {
        return getResourcePath(type).append(filename);
    }

    ResourceHandler<StaticMesh> ResourceManager::loadStaticMesh(const char* filename)
    {
        ResourceHandler<StaticMesh> staticMeshHandler;
        auto staticMeshId = utils::hashStr(filename);
        
        if (m_meshMap.find(staticMeshId) == m_meshMap.end())
        {
            ObjLoader loader;
            auto meshResourcePath = ResourceManager::getInstance().getResourcePath(ResourceType::MESH, filename);
            auto staticMesh = std::make_unique<StaticMesh>(filename);
            loader.loadFile(meshResourcePath, staticMesh->getVertices(), staticMesh->getIndices());
            m_meshMap[staticMeshId] = std::move(staticMesh);
        }
        
        staticMeshHandler.resourceId = staticMeshId;
        staticMeshHandler.resource = m_meshMap[staticMeshId].get();
        
        return staticMeshHandler;
    }

    ResourceHandler<StaticMesh> ResourceManager::createStaticMesh(const char* name, Vertex* vertices, uint32_t vertexCount)
    {
        ResourceHandler<StaticMesh> staticMeshHandler;
        auto staticMeshId = utils::hashStr(name);
        
        if (m_meshMap.find(staticMeshId) == m_meshMap.end())
        {
            auto staticMesh = std::make_unique<StaticMesh>(vertices, vertexCount);
            m_meshMap[staticMeshId] = std::move(staticMesh);
        }
        
        staticMeshHandler.resourceId = staticMeshId;
        staticMeshHandler.resource = m_meshMap[staticMeshId].get();
        
        return staticMeshHandler;
    }

    ResourceHandler<StaticMesh> ResourceManager::createStaticMesh(const char* name, Vertex* vertices, uint32_t vertexCount,
                                                        const uint32_t* indices, uint32_t indexCount)
    {
        ResourceHandler<StaticMesh> staticMeshHandler;
        auto staticMeshId = utils::hashStr(name);
        
        if (m_meshMap.find(staticMeshId) == m_meshMap.end())
        {
            auto staticMesh = std::make_unique<StaticMesh>(vertices, vertexCount, indices, indexCount);
            m_meshMap[staticMeshId] = std::move(staticMesh);
        }
        
        staticMeshHandler.resourceId = staticMeshId;
        staticMeshHandler.resource = m_meshMap[staticMeshId].get();
        
        return staticMeshHandler;        
    }

    ResourceHandler<StaticMesh> ResourceManager::getStaticMesh(uint32_t staticMeshId)
    {
        ResourceHandler<StaticMesh> staticMeshHandler;
        if (auto it = m_meshMap.find(staticMeshId); it != m_meshMap.end())
        {
            staticMeshHandler.resourceId = staticMeshId;
            staticMeshHandler.resource = it->second.get();
        }

        return staticMeshHandler;
    }

} // namespace comet
