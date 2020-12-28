#include <comet/framebuffer.h>

namespace comet
{

    class OpenglFramebuffer : public Framebuffer
    {
    public:
        OpenglFramebuffer(const FramebufferSpec& spec);
        virtual ~OpenglFramebuffer();

        OpenglFramebuffer(OpenglFramebuffer&& other);
        OpenglFramebuffer& operator=(OpenglFramebuffer&& other) noexcept;

        virtual uint32_t getBufferId() const override { return m_bufferId; }
        virtual void invalidate() override;
        virtual void bind() const override;
        virtual void unbind() const override;
    
    private:
        uint32_t m_bufferId{0};
        uint32_t m_colorAttachment{0};
        uint32_t m_depthAttachment{0};
    };
    
} // namespace comet
