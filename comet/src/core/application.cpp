#include <comet/application.h>
#include <comet/window.h>
#include <platforms/sfml/SFMLWindow.h>

#include <functional>
#include <chrono>
#include <thread>

namespace comet
{
    #define BIND_METHOD(m) std::bind(&m, this, std::placeholders::_1)

    Application::Application(const WindowSpec& spec)
    {
        init(spec);
        CM_CORE_LOG_DEBUG("Application contructor");
    }

    Application::~Application()
    {
        CM_CORE_LOG_DEBUG("Application destructor");
    };

    void Application::setFPSCap(unsigned int fpsCap)
    {
        m_fpsCap = fpsCap;
    }

    void Application::init(const WindowSpec& spec)
    {
        Log::init();
        
        CM_CORE_LOG_DEBUG("Initializing the application");
        m_window = Window::create(spec);

        // Deactivate VSync (slowness of window motion on Linux when VSync is on)
        m_window->setVSync(false);

        // set Event callback
        m_window->addEventCallback(BIND_METHOD(Application::onEventDispatch));

        m_isInitialized = true;
        CM_CORE_LOG_DEBUG("Finished initializing the application");
    }

    void Application::onEventDispatch(Event& e)
    {
        EventDispatcher dispatcher(e);

        dispatcher.dispatch<WindowResizedEvent>(BIND_METHOD(Application::onWindowResized));
        dispatcher.dispatch<WindowLostFocusEvent>(BIND_METHOD(Application::onWindowLostFocus));
        dispatcher.dispatch<WindowGainedFocusEvent>(BIND_METHOD(Application::onWindowGainedFocus));
        dispatcher.dispatch<KeyPressedEvent>(BIND_METHOD(Application::onKeyPressed));
        dispatcher.dispatch<KeyReleasedEvent>(BIND_METHOD(Application::onKeyReleased));
        dispatcher.dispatch<KeyTextEnteredEvent>(BIND_METHOD(Application::onTextEntered));
        dispatcher.dispatch<MouseMovedEvent>(BIND_METHOD(Application::onMouseMoved));
        dispatcher.dispatch<MouseEnteredWindowEvent>(BIND_METHOD(Application::onMouseEnteredWindow));
        dispatcher.dispatch<MouseLeftWindowEvent>(BIND_METHOD(Application::onMouseLeftWindow));
        dispatcher.dispatch<VerticalMouseWheelScrolledEvent>(BIND_METHOD(Application::onVerticalMouseWheelScrolled));
        dispatcher.dispatch<HorizontalMouseWheelScrolledEvent>(BIND_METHOD(Application::onHorizontalMouseWheelScrolled));
        dispatcher.dispatch<MouseButtonPressedEvent>(BIND_METHOD(Application::onMouseButtonPressed));
        dispatcher.dispatch<MouseButtonReleasedEvent>(BIND_METHOD(Application::onMouseButtonRelease));

        // Dispatch the event further
        onEvent(e);
    }

    bool Application::onEvent(Event& e) { return m_activeScene->onEvent(e); }
    bool Application::onWindowResized(WindowResizedEvent& e) { return m_activeScene->onWindowResized(e); }
    bool Application::onWindowLostFocus(WindowLostFocusEvent& e) { return m_activeScene->onWindowLostFocus(e); }
    bool Application::onWindowGainedFocus(WindowGainedFocusEvent& e) { return m_activeScene->onWindowGainedFocus(e); }
    bool Application::onKeyPressed(KeyPressedEvent& e) { return m_activeScene->onKeyPressed(e); }
    bool Application::onKeyReleased(KeyReleasedEvent& e) { return m_activeScene->onKeyReleased(e); }
    bool Application::onTextEntered(KeyTextEnteredEvent& e) { return m_activeScene->onTextEntered(e); }
    bool Application::onMouseMoved(MouseMovedEvent& e) { return m_activeScene->onMouseMoved(e); }
    bool Application::onMouseEnteredWindow(MouseEnteredWindowEvent& e) { return m_activeScene->onMouseEnteredWindow(e); }
    bool Application::onMouseLeftWindow(MouseLeftWindowEvent& e) { return m_activeScene->onMouseLeftWindow(e); }
    bool Application::onVerticalMouseWheelScrolled(VerticalMouseWheelScrolledEvent& e) { return m_activeScene->onVerticalMouseWheelScrolled(e); }
    bool Application::onHorizontalMouseWheelScrolled(HorizontalMouseWheelScrolledEvent& e) { return m_activeScene->onHorizontalMouseWheelScrolled(e); }
    bool Application::onMouseButtonPressed(MouseButtonPressedEvent& e) { return m_activeScene->onMouseButtonPressed(e); }
    bool Application::onMouseButtonRelease(MouseButtonReleasedEvent& e) { return m_activeScene->onMouseButtonRelease(e); }

    void Application::run()
    {
        CM_CORE_LOG_INFO("Start running the application");
        m_isRunning = true;

        CM_CORE_LOG_DEBUG("Calling onStart()");
        onStart();

        CM_CORE_LOG_DEBUG("Starting main loop");
        using namespace std::chrono;

        duration<double, std::nano> lag(0);
        duration<double, std::milli> fixedUpdateTime(m_fixedUpdateTime);
        duration<double, std::nano> elapsedTime;
        duration<float, std::milli> fpsCapTime(0.0f);
        auto previousTime = steady_clock::now();
        auto currentTime = previousTime;
        double deltaTime(0.0f);

        if (m_fpsCap)
        {
            fpsCapTime = duration<float, std::milli>(1000.0f / m_fpsCap);
            CM_CORE_LOG_DEBUG("FPS Cap Time set to: {}ms", fpsCapTime.count());
        }

        if (m_nextActiveScene == nullptr)
        {
            CM_LOG_FATAL("No Scene defined!");
        }
        else
        {
            // Game Loop
            while(!m_window->isCloseRequested())
            {
                if (m_nextActiveScene)
                {
                    if (m_activeScene)
                    {
                        m_activeScene->stop();
                    }

                    m_activeScene = m_nextActiveScene;
                    m_nextActiveScene = nullptr;
                    m_activeScene->start();
                }

                T_gameloop.resume();
                currentTime = steady_clock::now();
                elapsedTime = currentTime - previousTime;
                previousTime = currentTime;
                lag += elapsedTime;

                // INPUTS: Poll window events
                m_window->pollEvent();

                // FIXED UPDATES: Scene Fixed Update Callback
                T_fixed_udpate.resume();
                // Update as lag permits
                while(lag >= fixedUpdateTime)
                {
                    m_activeScene->onFixedUpdate(fixedUpdateTime.count());
                    lag -= fixedUpdateTime;
                }
                T_fixed_udpate.pause();

                // UPDATES: Scene Update Callback
                T_update.resume();
                deltaTime = duration_cast<duration<double, std::milli>>(elapsedTime).count();
                m_activeScene->update(deltaTime);

                // Application onUpdate Callback
                onUpdate();
                T_update.pause();

                // RENDER: Scene Rendering Callback
                T_render.resume();
                m_window->clearBuffers();
                m_activeScene->render();

                // Application Render Callback
                onRender();
                T_render.pause();

                m_window->swapBuffers();

                T_gameloop.pause();

                // Is FPS Cap needed?
                if (m_fpsCap)
                {
                    auto t1 = steady_clock::now();
                    auto waitTime = fpsCapTime - duration_cast<milliseconds>(t1 - currentTime);
                    if (waitTime.count() > 1.0f)
                    {
                        std::this_thread::sleep_for(waitTime);
                    }
                }
                else
                {
                    std::this_thread::sleep_for(microseconds(5));
                }
            }
        }
        
        if (m_activeScene)
        {
            m_activeScene->stop();
        }

        // Application onStop Callback
        onStop();

        m_window->close();
        CM_CORE_LOG_DEBUG("Exit main loop");
    }

} // namespace comet
