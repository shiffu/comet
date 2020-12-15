#pragma once

#include <entt/entt.hpp>

namespace comet
{
    class Entity;
    class Camera;

    class Scene
    {
        friend class Entity;
        friend class Renderer;

    public:
        Entity createEntity();

        void setCamera(Camera* camera) {}

        virtual void onStart() {}
        virtual void onShutdown() {}
        virtual void onUpdate(double deltaTime) {}
        virtual void onFixedUpdate(float fixedDeltaTime) {}

    public:
        // TEMPORARY (FOR TESTING)
        entt::registry m_registry;

    private:
        Camera* m_camera{nullptr};
    };
    
} // namespace comet
