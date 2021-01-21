#include <comet/scene.h>
#include <comet/entity.h>
#include <comet/components.h>
#include <comet/nativeScript.h>

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

    void Scene::start()
    {
        onStart();
        m_registry.view<NativeScriptComponent>().each([this](auto entityId, auto& scriptComponent)
        {
            instantiateNativeScriptComponent(scriptComponent);
        });
        reload();
    }

    void Scene::instantiateNativeScriptComponent(NativeScriptComponent& scriptComponent)
    {
        if (scriptComponent.instantiateScript)
        {
            scriptComponent.instance = scriptComponent.instantiateScript();
            scriptComponent.instance->onCreate();
        }
    }

    void Scene::reload()
    {
        m_renderer.setScene(this);
        m_renderer.allocateBuffersAndSetupLayouts();
        m_renderer.loadData();
    }

    void Scene::stop()
    {
        onStop();
        m_registry.view<NativeScriptComponent>().each([](auto entityId, auto& scriptComponent)
        {
            if (scriptComponent.instance)
            {
                scriptComponent.instance->onDestroy();
            }

            if (scriptComponent.destroyScript)
            {
                scriptComponent.destroyScript(&scriptComponent);
                scriptComponent.destroyScript = nullptr;
                scriptComponent.instance = nullptr;
            }
        });
    }

    void Scene::onUpdate(double deltaTime)
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

} // namespace comet
