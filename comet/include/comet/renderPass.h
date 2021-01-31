#pragma once

#include <comet/framebuffer.h>
#include <comet/renderer.h>

namespace comet
{
    
    struct RenderPassSpec
    {
        std::shared_ptr<Framebuffer> target;
    };
    
    class RenderPass
    {
    public:
        RenderPass() = default;
        RenderPass(const RenderPassSpec& spec, const Renderer& renderer)
            : m_spec(spec), m_renderer(renderer)
        {
            CM_CORE_LOG_DEBUG("RenderPass::RenderPass(const RenderPassSpec&, const Renderer&)");
        }

        ~RenderPass() = default;

        static std::shared_ptr<RenderPass> create(const RenderPassSpec& spec, const Renderer& renderer);

        void prepare();
        void run();

        const RenderPassSpec& getSpec() const { return m_spec; }
        RenderPassSpec& getSpec() { return m_spec; }
        std::shared_ptr<Framebuffer> getTarget() const { return m_spec.target; }
    
    private:
        RenderPassSpec m_spec;
        Renderer m_renderer;
    };

} // namespace comet
