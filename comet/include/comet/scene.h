#pragma once

#include <entt/entt.hpp>
#include <comet/cameraController.h>
#include <comet/renderer.h>

namespace comet
{
    class Entity;

    class Scene
    {
        friend class Entity;
        friend class Renderer;

    public:
        Entity createEntity();

        void setCameraController(CameraController* cameraController) { m_cameraController = cameraController; }
        CameraController* getCameraController() { return m_cameraController; }

        entt::registry& getRegistry() { return m_registry; }

        void addLight(Light* light) { m_renderer.addLight(light); }
        void prepare();
        void render();

        virtual void onStart() {}
        virtual void onShutdown() {}
        virtual void onUpdate(double deltaTime) {}
        virtual void onFixedUpdate(float fixedDeltaTime) {}

    private:
        entt::registry m_registry;
        Renderer m_renderer;
        CameraController* m_cameraController{nullptr};
    };
    
} // namespace comet
