#pragma once

#include <comet/event.h>
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
    
    class NativeScriptComponent;

    class Scene
    {
        friend class Entity;
        friend class Renderer;

    public:
        Scene(const char* name) : m_name(name) {}
        virtual ~Scene() = default;

        Entity createEntity(bool createDefaultComponents = true);
        void destroyEntity(Entity& entity);

        entt::registry& getRegistry() { return m_registry; }

        SceneStats& getStatistics() { return m_sceneStatistics; }
        const SceneStats& getStatistics() const { return m_sceneStatistics; }

        void setName(const std::string& name) { m_name = name; }
        const std::string& getName() const { return m_name; }

        void addLight(Light* light) { m_renderer.addLight(light); m_sceneStatistics.lightsCount++; }

        void clear();
        void start();
        void stop();
        void reload();

        void instantiateNativeScriptComponent(NativeScriptComponent& scriptComponent);

        virtual void onStart() {}
        virtual void onStop() {}
        virtual void onUpdate(double deltaTime);
        virtual void onFixedUpdate(double fixedDeltaTime) {}
        virtual void onRender() {}

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

    protected:
        SceneStats m_sceneStatistics;
        Renderer m_renderer;

    private:
        std::string m_name;
        entt::registry m_registry;
    };
    
} // namespace comet
