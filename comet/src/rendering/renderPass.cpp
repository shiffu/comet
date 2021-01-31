#include <comet/renderPass.h>

namespace comet
{
    
    std::shared_ptr<RenderPass> RenderPass::create(const RenderPassSpec& spec, const Renderer& renderer)
    {
        return std::make_shared<RenderPass>(spec, renderer);
    }

    void RenderPass::prepare()
    {
        m_renderer.prepare();
    }

    void RenderPass::run()
    {
        if (m_spec.target != nullptr)
        {
            m_spec.target->bind();
            m_spec.target->clear();

            m_renderer.reloadInstanceData();
            m_renderer.render();
            
            m_spec.target->unbind();
        }
    }

} // namespace comet
