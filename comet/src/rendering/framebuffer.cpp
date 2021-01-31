#include <comet/framebuffer.h>
#include <comet/graphicApiConfig.h>
#include <platforms/opengl/openglFramebuffer.h>

namespace comet
{

    std::shared_ptr<Framebuffer> Framebuffer::create(const FramebufferSpec& spec)
    {
        switch (GraphicApiConfig::getApiImpl())
        {
            case GraphicApiConfig::API::OPENGL:
                return std::make_unique<OpenglFramebuffer>(spec);
        }
        
        ASSERT(false, "Graphic API not supported for now!");
        return std::unique_ptr<OpenglFramebuffer>(nullptr);
    }

    void Framebuffer::setSpec(const FramebufferSpec& spec)
    {
        bool isInvalid{false};

        if (m_spec.width != spec.width || m_spec.height != spec.height)
        {
            isInvalid = true;
        }

        m_spec = spec;
        if (isInvalid) invalidate();
    }
    
} // namespace comet
