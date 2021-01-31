#pragma once

#include <glm/vec4.hpp>

namespace comet
{

    enum class FramebufferTextureFormat
    {
        NONE = 0,

        // Color
        RGBA8,
        // RGBA16F,
        // RGBA32F,

        // Depth & Stencil
        // STENCIL8,
        DEPTH24_STENCIL8,
        // DEPTH32F_STENCIL8,
        // DEPTH32F,
    };
    
    struct FramebufferTextureSpec
    {
        FramebufferTextureSpec() = default;
        FramebufferTextureSpec(FramebufferTextureFormat format)
            : format(format) {}

        FramebufferTextureFormat format{FramebufferTextureFormat::NONE};
        // TODO: implement wrap and filtering
    };

    struct FramebufferAttachments
    {
        FramebufferAttachments() = default;
        FramebufferAttachments(std::initializer_list<FramebufferTextureSpec> specs)
            : attachments(specs) {}

        std::vector<FramebufferTextureSpec> attachments;
    };

    struct FramebufferSpec
    {
        uint16_t width{0};
        uint16_t height{0};
        uint16_t samples{1};
        glm::vec4 clearColor{0.0f, 0.0f, 0.0f, 1.0f};
        bool swapChainTarget{false};
        FramebufferAttachments attachmentSet;
    };
    
    class Framebuffer
    {
    public:
        Framebuffer() = default;
        Framebuffer(const FramebufferSpec& spec) : m_spec(spec) {}
        virtual ~Framebuffer() {}

        static std::shared_ptr<Framebuffer> create(const FramebufferSpec& spec);

        Framebuffer(const Framebuffer&) = delete;
        void operator=(const Framebuffer&) = delete;

        const FramebufferSpec& getSpec() const { return m_spec; }
        void setSpec(const FramebufferSpec& spec);

        virtual uint32_t getColorAttachmentId(size_t index = 0) const = 0 ;
        virtual void invalidate() = 0;
        virtual void clear() const = 0;
        virtual void bind() const = 0;
        virtual void unbind() const = 0;
    
    protected:
        FramebufferSpec m_spec;
    };
    
} // namespace comet
