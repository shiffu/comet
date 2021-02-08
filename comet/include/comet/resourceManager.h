#pragma once

#include <comet/singleton.h>
#include <string>
#include <memory>
#include <unordered_map>
#include <experimental/filesystem>
#include <comet/staticMesh.h>

namespace comet
{
    enum class ResourceType
    {
        NONE = 0,
        SHADER,
        TEXTURE,
        FONT,
        MESH,
        AUDIO
    };

    namespace fs = std::experimental::filesystem;


    template<typename T>
    struct ResourceHandler
    {
        uint32_t resourceId{0};
        T* resource{nullptr} ;
    };


    class ResourceManager : public Singleton<ResourceManager>
    {
    public:
        void setRootPath(const char* rootPath) { m_rootPath = rootPath; }

        fs::path getResourcePath(ResourceType type) const;
        fs::path getResourcePath(ResourceType type, const char* filename) const;

        ResourceHandler<StaticMesh> loadStaticMesh(const char* filename);
        ResourceHandler<StaticMesh> createStaticMesh(const char* name, Vertex* vertices, uint32_t vertexCount);
        ResourceHandler<StaticMesh> createStaticMesh(const char* name, Vertex* vertices, uint32_t vertexCount,
                    const uint32_t* indices, uint32_t indexCount);
        ResourceHandler<StaticMesh> getStaticMesh(uint32_t staticMeshId);

    private:
        const char* SHADER_SUBFOLDER = "shaders";
        const char* TEXTURE_SUBFOLDER = "textures";
        const char* FONT_SUBFOLDER = "fonts";
        const char* MESH_SUBFOLDER = "meshes";
        const char* AUDIO_SUBFOLDER = "audio";

    private:
        // TODO(jcp): Get the default root value from the expected packaging structure
        std::string m_rootPath{"../../comet/resources"};
        std::unordered_map<uint32_t, std::unique_ptr<StaticMesh>> m_meshMap{};
    };
    
} // namespace comet
