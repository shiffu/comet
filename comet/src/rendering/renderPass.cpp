#include <comet/renderPass.h>

namespace comet
{
    
    std::unique_ptr<RenderPass> RenderPass::create(const RenderPassSpec& spec, std::unique_ptr<Renderer>&& renderer)
    {
        return std::make_unique<RenderPass>(spec, std::move(renderer));
    }

    RenderPass& RenderPass::operator=(RenderPass&& other)
    {
        if (&other == this)
        {
            return *this;
        }

        m_spec = std::move(other.m_spec);
        m_renderer = std::move(other.m_renderer);
    }

    void RenderPass::prepare()
    {
        m_renderer->prepare();
    }

    void RenderPass::invalidate()
    {
        if (m_spec.target != nullptr)
        {
            m_spec.target->invalidate();
        }
    }

    void RenderPass::run()
    {
        // When renderPasses are chained together
        // TODO(jcp): Implement chained RenderPass (PostProcessing)
        if (m_spec.inputFramebuffer != nullptr)
        {
        }
        
        if (m_spec.target != nullptr)
        {
            m_spec.target->bind();
            m_spec.target->clear();

            m_renderer->reloadData();
            m_renderer->render();
            
            m_spec.target->unbind();

            // For SwapChain target, render the framebuffer texture on the screen
            if (m_spec.target->getSpec().swapChainTarget == true)
            {
                m_spec.target->renderToScreen();
            }
        }
    }

} // namespace comet
