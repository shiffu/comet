#pragma once

#include <comet/framebuffer.h>
#include <comet/renderer.h>

namespace comet
{
    
    struct RenderPassSpec
    {
        std::shared_ptr<Framebuffer> target;
        Framebuffer* inputFramebuffer{nullptr};
    };
    
    class RenderPass
    {
    public:
        RenderPass() = default;
        RenderPass(const RenderPassSpec& spec, std::unique_ptr<Renderer>&& renderer)
            : m_spec(spec), m_renderer(std::move(renderer)) {}

        ~RenderPass() = default;

        RenderPass(RenderPass&& other)
            : m_spec(std::move(other.m_spec)),
            m_renderer(std::move(other.m_renderer)) {}

        RenderPass& operator=(RenderPass&& other);

        static std::unique_ptr<RenderPass> create(const RenderPassSpec& spec, std::unique_ptr<Renderer>&& renderer);

        void prepare();
        void invalidate();
        void run();

        const RenderPassSpec& getSpec() const { return m_spec; }
        RenderPassSpec& getSpec() { return m_spec; }
        std::shared_ptr<Framebuffer> getTarget() const { return m_spec.target; }
    
    private:
        RenderPassSpec m_spec;
        std::unique_ptr<Renderer> m_renderer;
    };

} // namespace comet
