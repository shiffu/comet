#include <comet/texture.h>
#include <comet/assert.h>
#include <comet/graphicApiConfig.h>
#include <platforms/opengl/openglTexture.h>

namespace comet
{

    std::unique_ptr<Texture2D> Texture2D::create(uint32_t textureId)
    {
        switch (GraphicApiConfig::getApiImpl())
        {
            case GraphicApiConfig::API::OPENGL:
                return std::make_unique<OpenglTexture2D>(textureId);
        }
        
        ASSERT(false, "Graphic API not supported for now!");
        return std::unique_ptr<OpenglTexture2D>(nullptr);
    }

    std::unique_ptr<Texture2D> Texture2D::create(const char* filename /*= nullptr*/)
    {
        switch (GraphicApiConfig::getApiImpl())
        {
            case GraphicApiConfig::API::OPENGL:
                return std::make_unique<OpenglTexture2D>(filename);
        }
        
        ASSERT(false, "Graphic API not supported for now!");
        return std::unique_ptr<OpenglTexture2D>(nullptr);
    }

    std::unique_ptr<Texture2DArray> Texture2DArray::create()
    {
        switch (GraphicApiConfig::getApiImpl())
        {
            case GraphicApiConfig::API::OPENGL:
                return std::make_unique<OpenglTexture2DArray>();
        }
        
        ASSERT(false, "Graphic API not supported for now!");
        return std::unique_ptr<OpenglTexture2DArray>(nullptr);
    }

} // namespace comet
