#pragma once

#include <core/singleton.h>
#include <string>
#include <experimental/filesystem>

namespace comet
{
    enum class ResourceType
    {
        NONE = 0,
        SHADER,
        TEXTURE,
        MESH,
        AUDIO
    };

    namespace fs = std::experimental::filesystem;

    class ResourceManager : public Singleton<ResourceManager>
    {
    public:
        void setRootPath(const char* rootPath) { m_rootPath = rootPath; }

        fs::path getResourcePath(ResourceType type) const;
        fs::path getResourcePath(ResourceType type, const char* filename) const;

    private:
        const char* SHADER_SUBFOLDER = "shaders";
        const char* TEXTURE_SUBFOLDER = "textures";
        const char* MESH_SUBFOLDER = "meshes";
        const char* AUDIO_SUBFOLDER = "audio";

    private:
        // TODO: Get the default root value from the expected packaging structure
        std::string m_rootPath{"../../comet/resources"};
    };
    
} // namespace comet
