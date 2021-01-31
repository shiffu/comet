#include <comet/scene.h>
#include <comet/entity.h>
#include <comet/components.h>
#include <comet/nativeScript.h>

#include <algorithm>

namespace comet
{
    
    Entity Scene::createEntity(bool createDefaultComponents /*= true*/)
    {
        Entity entity{m_registry.create(), this};
        if (createDefaultComponents)
        {
            entity.addComponent<TransformComponent>();
            entity.addComponent<NameComponent>();
        }

        m_sceneStatistics.entitiesCount++;
        return entity;
    }

    void Scene::destroyEntity(Entity& entity)
    {
        m_registry.destroy(entity.getId());
        m_sceneStatistics.entitiesCount--;
    }

    void Scene::clear()
    {
        m_registry.clear();
        reload();
    }

    void Scene::addLight(std::unique_ptr<Light>&& light)
    {
        m_lights.push_back(std::move(light));
        m_sceneStatistics.lightsCount++;
    }

    uint32_t Scene::addRenderPass(const RenderPassSpec& renderPassSpec, const Renderer& renderer)
    {
        m_renderPasses.push_back(RenderPass::create(renderPassSpec, renderer));
        
        return m_renderPasses.size() - 1;
    }

    void Scene::removeRenderPass(size_t index)
    {
        ASSERT(index < m_renderPasses.size(), "out of bound index");
        m_renderPasses.erase(m_renderPasses.begin() + index);
    }

    void Scene::removeAllRenderPasses()
    {
        CM_CORE_LOG_DEBUG("Scene::removeAllRenderPasses()");
        m_renderPasses.erase(m_renderPasses.begin(), m_renderPasses.end());
    }

    std::shared_ptr<RenderPass> Scene::getRenderPass(size_t index)
    {
        ASSERT(index < m_renderPasses.size(), "out of bound index");
        return m_renderPasses[index];
    }

    std::shared_ptr<RenderPass> Scene::getSwapChainTargetRenderPass() const
    {
        auto swapChainTargerRenderPassIt = std::find_if(m_renderPasses.begin(), m_renderPasses.end(), [](auto& renderPass)
        {
            return renderPass->getSpec().target->getSpec().swapChainTarget;
        });

        ASSERT(swapChainTargerRenderPassIt != m_renderPasses.end(), "No SwapChain Target found!");
        return *swapChainTargerRenderPassIt;
    }

    void Scene::start()
    {
        CM_CORE_LOG_DEBUG("Start Scene: {}", m_name);
        onStart();
        for (auto renderPass : m_renderPasses)
        {
            renderPass->prepare();
        }

        m_registry.view<NativeScriptComponent>().each([this](auto entityId, auto& scriptComponent)
        {
            instantiateNativeScriptComponent(scriptComponent);
        });
        reload();
    }

    void Scene::instantiateNativeScriptComponent(NativeScriptComponent& scriptComponent)
    {
        if (scriptComponent.instantiateScript && scriptComponent.instance == nullptr)
        {
            scriptComponent.instance = scriptComponent.instantiateScript();
            scriptComponent.instance->onCreate();
        }
    }

    void Scene::reload()
    {
        for (auto renderPass : m_renderPasses)
        {
            renderPass->prepare();
        }
    }

    void Scene::stop()
    {
        CM_CORE_LOG_DEBUG("Stop Scene: {}", m_name);
        onStop();
        removeAllRenderPasses();
        m_registry.view<NativeScriptComponent>().each([](auto entityId, auto& scriptComponent)
        {
            if (scriptComponent.instance)
            {
                scriptComponent.instance->onDestroy();
                scriptComponent.instance = nullptr;
            }

            if (scriptComponent.destroyScript)
            {
                scriptComponent.destroyScript(&scriptComponent);
            }
        });
    }

    void Scene::onUpdate(double deltaTime)
    {
        if (m_runtime)
        {
            m_registry.view<NativeScriptComponent>().each([deltaTime, this](auto entityId, auto& scriptComponent)
            {
                if (scriptComponent.instance)
                {
                    Entity entity{entityId, this};
                    scriptComponent.instance->onUpdate(entity, deltaTime);
                }
            });
        }
    }

    void Scene::render()
    {
        for (auto renderPass : m_renderPasses)
        {
            renderPass->run();
        }
    }

} // namespace comet
