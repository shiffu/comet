#pragma once

#include <comet/texture.h>

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
    
    struct FramebufferAttachmentSpec
    {
        FramebufferAttachmentSpec() = default;
        FramebufferAttachmentSpec(FramebufferTextureFormat format)
            : format(format) {}
        FramebufferAttachmentSpec(FramebufferTextureFormat format, glm::vec4 clearColor)
            : format(format), clearColor(clearColor) {}

        FramebufferTextureFormat format{FramebufferTextureFormat::NONE};
        glm::vec4 clearColor{0.0f, 0.0f, 0.0f, 1.0f};

        // TODO(jcp): implement wrap and filtering
    };

    struct FramebufferAttachmentSet
    {
        FramebufferAttachmentSet() = default;
        FramebufferAttachmentSet(std::initializer_list<FramebufferAttachmentSpec> specs)
            : attachments(specs) {}

        std::vector<FramebufferAttachmentSpec> attachments;
    };

    struct FramebufferSpec
    {
        uint16_t width{0};
        uint16_t height{0};
        uint16_t samples{1};
        bool swapChainTarget{false};
        FramebufferAttachmentSet attachmentSet;
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

        virtual int getColorAttachmentId(size_t index = 0) const = 0;
        virtual std::unique_ptr<Texture2D> getColorAttachmentTexture(size_t index = 0) const = 0;

        virtual void invalidate() = 0;
        virtual void renderToScreen() = 0;
        virtual void clear() const = 0;
        virtual void bind() const = 0;
        virtual void unbind() const = 0;
    
    protected:
        FramebufferSpec m_spec;
    };
    
} // namespace comet
