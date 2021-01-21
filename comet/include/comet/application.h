#pragma once

#include <comet/window.h>
#include <comet/event.h>
#include <comet/scene.h>
#include <comet/timer.h>

#include <memory>

namespace comet
{

    class Application
    {
    public:

        // TODO: Find a proper way to make Application a Singleton
        // Here we need the client to actually define this static function (not ideal)
        static Application* getInstance();

        Application(const Application&) = delete;
        Application& operator=(const Application&) = delete;

        void onEventDispatch(Event& e);
        virtual void run();
        
        Window* getWindow() const { return m_window; }

        // Configuration
        void setFPSCap(unsigned int fpsCap);

    protected:
        Application(const WindowSpec& spec = WindowSpec());
        virtual ~Application();

        Scene* getActiveScene() noexcept { return m_activeScene; }

        template<typename T>
        void setActiveScene(T* scene) noexcept { m_nextActiveScene = scene; }

        // Event Callbacks
        virtual bool onEvent(Event& e);   // Generic event handler
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

        virtual void onStart() {}
        virtual void onStop() {}
        virtual void onUpdate() {}
        virtual void onRender() {}

    private:
        void init(const WindowSpec& spec = WindowSpec());

    private:
        bool m_isRunning{false};
        bool m_isInitialized{false};

        Window* m_window{nullptr};

        Scene* m_activeScene{nullptr};
        Scene* m_nextActiveScene{nullptr};
        Scene* m_previousScene{nullptr};

        unsigned int m_fpsCap{0};
        

        // Fixed update time in ms (used to onFixedUpdate function call)
        // TODO: Make this fixed update time an application config param
        float m_fixedUpdateTime{20.0f};

        // Timers
        Timer T_gameloop{"GameLoop - Total", true};
        Timer T_fixed_udpate{"GameLoop - FixedUpdate", true};
        Timer T_update{"GameLoop - Update", true};
        Timer T_render{"GameLoop - Render", true};
    };

} // namespace comet
