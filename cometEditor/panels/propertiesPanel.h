#pragma once

#include <comet/scene.h>
#include <comet/entity.h>

namespace comet
{

    class PropertiesPanel
    {
    public:
        PropertiesPanel() = default;
        ~PropertiesPanel() = default;

        void onDraw(Scene& activeScene, Entity& selectedEntity);

    private:
        void drawNameComponent(Entity& entity);
        void drawTransformComponent(Entity& entity);
        void drawMeshComponent(Entity& entity);
        void drawMaterialComponent(Entity& entity);

        void drawVec3(const char* label, glm::vec3& vec, float firstColWidth, float resetValue = 0.0f);

    private:
        Entity m_selectedEntity;
    };

} // namespace comet
