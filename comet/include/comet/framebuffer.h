#pragma once

#include <cstdint>
#include <memory>

namespace comet
{

    struct FramebufferSpec
    {
        uint32_t width{0};
        uint32_t height{0};
    };
    
    class Framebuffer
    {
    public:
        Framebuffer() = default;
        Framebuffer(const FramebufferSpec& spec) : m_spec(spec) {}
        virtual ~Framebuffer() {}

        static std::unique_ptr<Framebuffer> create(const FramebufferSpec& spec);

        Framebuffer(const Framebuffer&) = delete;
        void operator=(const Framebuffer&) = delete;

        const FramebufferSpec& getSpec() const { return m_spec; }
        void setSpec(const FramebufferSpec& spec) { m_spec = spec; }

        virtual uint32_t getBufferId() const = 0 ;
        virtual void invalidate() = 0;
        virtual void bind() const = 0;
        virtual void unbind() const = 0;
    
    protected:
        FramebufferSpec m_spec;
    };
    
} // namespace comet
