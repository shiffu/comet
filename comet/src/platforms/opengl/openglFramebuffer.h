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

        virtual uint32_t getColorAttachmentId(size_t index = 0) const override
        {
            ASSERT(index < m_colorAttachmentIds.size(), "Invalid index");
            return m_colorAttachmentIds[index];
        }

        virtual void invalidate() override;
        virtual void clear() const override;
        virtual void bind() const override;
        virtual void unbind() const override;
    
    private:
        uint32_t m_bufferId{0};
        std::vector<FramebufferTextureSpec> m_colorAttachmentSpecs;
        FramebufferTextureSpec m_depthAttachmentSpec{FramebufferTextureFormat::NONE};
        std::vector<uint32_t> m_colorAttachmentIds;
        uint32_t m_depthAttachmentId{0};
    };
    
} // namespace comet
