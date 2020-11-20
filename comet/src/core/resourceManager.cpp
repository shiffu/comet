#include <core/resourceManager.h>
#include <comet/log.h>

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

} // namespace comet
