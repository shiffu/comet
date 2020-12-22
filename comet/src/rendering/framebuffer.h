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
        virtual ~Framebuffer() {}

        static std::unique_ptr<Framebuffer> create(const FramebufferSpec& spec);

        Framebuffer(const Framebuffer&) = delete;
        void operator=(const Framebuffer&) = delete;


        virtual uint32_t getBufferId() const = 0 ;
        virtual void invalidate() = 0;
        virtual void bind() const = 0;
        virtual void unbind() const = 0;
    };
    
} // namespace comet
