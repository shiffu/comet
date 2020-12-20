#include <rendering/commandBuffer.h>
#include <comet/assert.h>
#include <comet/graphicApiConfig.h>
#include <platforms/opengl/openglCommandBuffer.h>

namespace comet
{

    std::unique_ptr<CommandBuffer> CommandBuffer::create(uint32_t usage)
    {
        switch (GraphicApiConfig::getApiImpl())
        {
            case GraphicApiConfig::API::OPENGL:
                return std::make_unique<OpenglCommandBuffer>(usage);
        }
        
        ASSERT(false, "Graphic API not supported for now!");
        return std::unique_ptr<OpenglCommandBuffer>(nullptr);
    }

}