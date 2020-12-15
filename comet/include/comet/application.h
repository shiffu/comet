#pragma once

#include <comet/window.h>
#include <comet/event.h>
#include <comet/scene.h>
#include <memory>

namespace comet
{

    class ImguiWrapper;
    class Scene;

    class Application
    {
    public:

        // TODO: Find a proper way to make Application a Singleton
        // Here we need the client to actually define this static function (not ideal)
        static Application* getInstance();

        Application(const Application&) = delete;
        Application& operator=(const Application&) = delete;

        void onEvent(Event& e);
        virtual void run();
        
        Window& getWindow() const { return *m_window; }

        // Configuration
        void setFPSCap(unsigned int fpsCap);

    protected:
        Application(const WindowSpec& spec = WindowSpec());
        virtual ~Application();

        Scene& getActiveScene() noexcept { return m_activeScene; }

        // Event Callbacks
        virtual bool onWindowResized(WindowResizedEvent& e);
        virtual bool onWindowLostFocus(WindowLostFocusEvent& e);
        virtual bool onWindowGainedFocus(WindowGainedFocusEvent& e);
        virtual bool onKeyPressed(KeyPressedEvent& e);
        virtual bool onKeyReleased(KeyReleasedEvent& e);
        virtual bool onTextEntered(KeyTextEnteredEvent& e);
        virtual bool onMouseMoved(MouseMovedEvent& e);
        virtual bool onMouseEnteredWindow(MouseEnteredWindowEvent& e);
        virtual bool onMouseLeftWindow(MouseLeftWindowEvent& e);
        virtual bool onVerticalMouseWheelScrolled(VerticalMouseWheelScrolledEvent& e);
        virtual bool onHorizontalMouseWheelScrolled(HorizontalMouseWheelScrolledEvent& e);
        virtual bool onMouseButtonPressed(MouseButtonPressedEvent& e);
        virtual bool onMouseButtonRelease(MouseButtonReleasedEvent& e);

        // Imgui Callback
        virtual void onImGuiDraw();

    private:
        void init(const WindowSpec& spec = WindowSpec());

        virtual void onStart();
        virtual void onUpdate(double deltaTime);
        virtual void onFixedUpdate(float fixedDeltaTime);
        virtual void onRender();

        void onImGuiDebugDraw();

    private:
        bool m_isRunning{false};
        bool m_isInitialized{false};
        Window* m_window{nullptr};
        Scene m_activeScene;
        unsigned int m_fpsCap{0};
        // Fixed update time in ms (used to onFixedUpdate function call)
        float m_fixedUpdateTime{20.0f};
        std::unique_ptr<ImguiWrapper> m_imguiWrapper;
    };

} // namespace comet

