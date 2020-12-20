#include <rendering/vertexBuffer.h>
#include <comet/assert.h>
#include <comet/graphicApiConfig.h>
#include <platforms/opengl/openglVertexBuffer.h>

namespace comet
{

    std::unique_ptr<VertexBuffer> VertexBuffer::create(uint32_t usage)
    {
        switch (GraphicApiConfig::getApiImpl())
        {
            case GraphicApiConfig::API::OPENGL:
                return std::make_unique<OpenglVertexBuffer>(usage);
        }
        
        ASSERT(false, "Graphic API not supported for now!");
        return std::unique_ptr<OpenglVertexBuffer>(nullptr);
    }

}