#include <comet/scene.h>
#include <comet/entity.h>

namespace comet
{
    
    Entity Scene::createEntity()
    {
        Entity entity{ m_registry.create(), this};
        m_sceneStatistics.entitiesCount++;
        return entity;
    }

    void Scene::prepare()
    {
        m_renderer.addScene(this);
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
