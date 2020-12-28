#pragma once

#include <comet/scene.h>
#include <comet/entity.h>

namespace comet
{

    class SceneHierarchyPanel
    {
    public:
        SceneHierarchyPanel() = default;
        ~SceneHierarchyPanel() = default;

        void onDraw(Scene& activeScene);
        Entity& getSelectedEntity() { return m_selectedEntity; }

    private:
        void drawEntity(Entity entity);

    private:
        Entity m_selectedEntity{entt::null, nullptr};
    };

} // namespace comet
