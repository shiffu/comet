#pragma once

#include <comet/cameraController.h>
#include <comet/renderer.h>

#include <entt/entt.hpp>

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
        Scene() = default;
        virtual ~Scene() = default;

        Entity createEntity();
        void destroyEntity(Entity& entity);

        void setCameraController(CameraController* cameraController) { m_cameraController = cameraController; }
        CameraController* getCameraController() { return m_cameraController; }

        entt::registry& getRegistry() { return m_registry; }

        void addLight(Light* light) { m_renderer.addLight(light); m_sceneStatistics.lightsCount++; }

        SceneStats& getStatistics() { return m_sceneStatistics; }
        const SceneStats& getStatistics() const { return m_sceneStatistics; }

        void start();
        void stop();
        void reload();
        void render();

        virtual void onStart() {}
        virtual void onStop() {}
        virtual void onUpdate(double deltaTime);
        virtual void onFixedUpdate(double fixedDeltaTime) {}
        virtual void onBeginRender() {}
        virtual void onEndRender() {}

        // Event Callbacks
        virtual bool onEvent(Event& e) { return true; } // Generic event handler
        virtual bool onWindowResized(WindowResizedEvent& e) {return true; }
        virtual bool onWindowLostFocus(WindowLostFocusEvent& e) {return true; }
        virtual bool onWindowGainedFocus(WindowGainedFocusEvent& e) {return true; }
        virtual bool onKeyPressed(KeyPressedEvent& e) {return true; }
        virtual bool onKeyReleased(KeyReleasedEvent& e) {return true; }
        virtual bool onTextEntered(KeyTextEnteredEvent& e) {return true; }
        virtual bool onMouseMoved(MouseMovedEvent& e) {return true; }
        virtual bool onMouseEnteredWindow(MouseEnteredWindowEvent& e) {return true; }
        virtual bool onMouseLeftWindow(MouseLeftWindowEvent& e) {return true; }
        virtual bool onVerticalMouseWheelScrolled(VerticalMouseWheelScrolledEvent& e) {return true; }
        virtual bool onHorizontalMouseWheelScrolled(HorizontalMouseWheelScrolledEvent& e) {return true; }
        virtual bool onMouseButtonPressed(MouseButtonPressedEvent& e) {return true; }
        virtual bool onMouseButtonRelease(MouseButtonReleasedEvent& e) {return true; }

        // Imgui Callback
        virtual void onImGuiDraw() {}

    private:
        entt::registry m_registry;
        Renderer m_renderer;
        CameraController* m_cameraController{nullptr};
        SceneStats m_sceneStatistics;
    };
    
} // namespace comet
