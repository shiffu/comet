#include <comet/framebuffer.h>

#include <rendering/vertexBuffer.h>
#include <rendering/vertexArray.h>
#include <comet/shaderRegistry.h>

namespace comet
{

    class OpenglFramebuffer : public Framebuffer
    {
    public:
        OpenglFramebuffer(const FramebufferSpec& spec);
        virtual ~OpenglFramebuffer();

        OpenglFramebuffer(OpenglFramebuffer&& other);
        OpenglFramebuffer& operator=(OpenglFramebuffer&& other) noexcept;

        virtual int getColorAttachmentId(size_t index = 0) const override
        {
            ASSERT(index < m_colorAttachmentIds.size(), "Invalid index");
            return m_colorAttachmentIds[index];
        }
        
        virtual std::unique_ptr<Texture2D> getColorAttachmentTexture(size_t index = 0) const override;

        virtual void invalidate() override;
        virtual void renderToScreen() override;
        virtual void clear() const override;
        virtual void bind() const override;
        virtual void unbind() const override;
    
    private:
        uint32_t m_bufferId{0};
        std::vector<FramebufferAttachmentSpec> m_colorAttachmentSpecs;
        FramebufferAttachmentSpec m_depthAttachmentSpec{FramebufferTextureFormat::NONE};
        std::vector<uint32_t> m_colorAttachmentIds;
        uint32_t m_depthAttachmentId{0};
        Shader* m_shader{nullptr};
        std::unique_ptr<VertexBuffer> m_vbo;
        std::unique_ptr<VertexArray> m_vao;
    };
    
} // namespace comet
