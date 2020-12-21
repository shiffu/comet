#pragma once

#include <entt/entt.hpp>
#include <comet/cameraController.h>
#include <comet/renderer.h>

namespace comet
{
    class Entity;

    struct SceneStats
    {
        uint16_t lightsCount{0};
        uint32_t entitiesCount{0};
        uint32_t verticesCount{0};
        uint32_t indicesCount{0};
        uint32_t drawCalls{0};
        uint32_t drawCommandsCount{0};

        SceneStats& clear()
        {
            lightsCount = 0;
            entitiesCount = 0;
            verticesCount = 0;
            indicesCount = 0;
            drawCalls = 0;
            drawCommandsCount = 0;

            return *this;
        }
    };
    
    class Scene
    {
        friend class Entity;
        friend class Renderer;

    public:
        Entity createEntity();

        void setCameraController(CameraController* cameraController) { m_cameraController = cameraController; }
        CameraController* getCameraController() { return m_cameraController; }

        entt::registry& getRegistry() { return m_registry; }

        void addLight(Light* light) { m_renderer.addLight(light); m_sceneStatistics.lightsCount++; }
        void prepare();
        void render();

        SceneStats& getStatistics() { return m_sceneStatistics; }
        const SceneStats& getStatistics() const { return m_sceneStatistics; }

        virtual void onStart() {}
        virtual void onShutdown() {}
        virtual void onUpdate(double deltaTime) {}
        virtual void onFixedUpdate(float fixedDeltaTime) {}

    private:
        entt::registry m_registry;
        Renderer m_renderer;
        CameraController* m_cameraController{nullptr};
        SceneStats m_sceneStatistics;
    };
    
} // namespace comet
