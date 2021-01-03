#include <comet/scene.h>
#include <comet/entity.h>
#include <comet/nativeScript.h>

namespace comet
{
    
    Entity Scene::createEntity()
    {
        Entity entity{m_registry.create(), this};
        entity.addComponent<TransformComponent>();
        entity.addComponent<NameComponent>();

        m_sceneStatistics.entitiesCount++;
        return entity;
    }

    void Scene::destroyEntity(Entity& entity)
    {
        m_registry.destroy(entity.getId());
        m_sceneStatistics.entitiesCount--;
    }

    void Scene::start()
    {
        onStart();
        m_registry.view<NativeScriptComponent>().each([](auto entityId, auto& scriptComponent)
        {
            scriptComponent.instance = scriptComponent.instantiateScript();
            scriptComponent.instance->onCreate();
        });
        reload();
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
            scriptComponent.destroyScript(&scriptComponent);
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

    void Scene::render()
    {
        m_renderer.reloadInstanceData();
        glm::mat4 viewMatrix{1.0f};
        glm::mat4 projectionMatrix{1.0f};

        // Checking for CameraController presence
        // Default scene does not have a CameraController for instance
        if (m_cameraController)
        {
            viewMatrix = m_cameraController->getView();
            projectionMatrix = m_cameraController->getProjection();
        }

        m_renderer.render(viewMatrix, projectionMatrix);
    }

} // namespace comet
