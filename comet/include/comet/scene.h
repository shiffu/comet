#pragma once

#include <comet/event.h>
#include <comet/renderer.h>
#include <comet/light.h>
#include <comet/renderPass.h>

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
        Scene(const char* name, bool runtime = true) : m_name(name), m_runtime(runtime) {}
        virtual ~Scene() = default;

        Entity createEntity(bool createDefaultComponents = true);
        void destroyEntity(Entity& entity);

        entt::registry& getRegistry() { return m_registry; }

        SceneStats& getStatistics() { return m_sceneStatistics; }
        const SceneStats& getStatistics() const { return m_sceneStatistics; }

        void setName(const std::string& name) { m_name = name; }
        const std::string& getName() const { return m_name; }

        void addLight(std::unique_ptr<Light>&& light);
        const std::vector<std::unique_ptr<Light>>& getLights() { return m_lights; }

        uint32_t addRenderPass(const RenderPassSpec& renderPassSpec, const Renderer& renderer);
        void removeRenderPass(size_t index);
        void removeAllRenderPasses();

        virtual glm::mat4 getViewMatrix() = 0;
        virtual glm::mat4 getProjectionMatrix() = 0;

        std::shared_ptr<RenderPass> getRenderPass(size_t index);
        std::shared_ptr<RenderPass> getSwapChainTargetRenderPass() const;
        const std::vector<std::shared_ptr<RenderPass>>& getRenderPasses() { return m_renderPasses; }

        void clear();
        void start();
        void stop();
        void reload();
        void render();

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
        entt::registry m_registry;

    private:
        std::string m_name;
        bool m_runtime;
        std::vector<std::unique_ptr<Light>> m_lights{};
        std::vector<std::shared_ptr<RenderPass>> m_renderPasses{};
    };
    
} // namespace comet
