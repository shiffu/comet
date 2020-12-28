#include <comet/scene.h>
#include <comet/entity.h>

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

    void Scene::prepare()
    {
        m_renderer.setScene(this);
        m_renderer.allocateBuffersAndSetupLayouts();
        m_renderer.loadData();
    }

    void Scene::render()
    {
        m_renderer.reloadInstanceData();
        auto viewMatrix = m_cameraController->getView();
        auto& projectionMatrix = m_cameraController->getProjection();

        m_renderer.render(viewMatrix, projectionMatrix);
    }

} // namespace comet
