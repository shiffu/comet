#include <rendering/indexBuffer.h>
#include <comet/assert.h>
#include <comet/graphicApiConfig.h>
#include <platforms/opengl/openglIndexBuffer.h>

namespace comet
{

    std::unique_ptr<IndexBuffer> IndexBuffer::create(uint32_t usage)
    {
        switch (GraphicApiConfig::getApiImpl())
        {
            case GraphicApiConfig::API::OPENGL:
                return std::make_unique<OpenglIndexBuffer>(usage);
        }
        
        ASSERT(false, "Graphic API not supported for now!");
        return std::unique_ptr<OpenglIndexBuffer>(nullptr);
    }

}