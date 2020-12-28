#include <comet/framebuffer.h>
#include <comet/assert.h>
#include <comet/graphicApiConfig.h>
#include <platforms/opengl/openglFramebuffer.h>

namespace comet
{

    std::unique_ptr<Framebuffer> Framebuffer::create(const FramebufferSpec& spec)
    {
        switch (GraphicApiConfig::getApiImpl())
        {
            case GraphicApiConfig::API::OPENGL:
                return std::make_unique<OpenglFramebuffer>(spec);
        }
        
        ASSERT(false, "Graphic API not supported for now!");
        return std::unique_ptr<OpenglFramebuffer>(nullptr);
    }
    
} // namespace comet
