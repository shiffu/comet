#include <glad/glad.h>
#include <iostream>
#include <functional>
#include <chrono>
#include <thread>
#include <comet/log.h>
#include <comet/application.h>
#include <comet/window.h>
#include <platforms/sfml/SFMLWindow.h>
#include <core/asserts.h>

namespace comet
{
    #define BIND_METHOD(m) std::bind(&m, this, std::placeholders::_1)

    static void APIENTRY glDebugCallback(GLenum source,
                                        GLenum type,
                                        GLuint id,
                                        GLenum severity,
                                        GLsizei length,
                                        const GLchar* message,
                                        const void* userParam)
    {
        std::string msgType;
        bool error_category = false;
        bool warn_category = false;
        switch(type)
        {
            case GL_DEBUG_TYPE_ERROR:
                msgType = "ERROR";
                error_category = true;
            break;
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
                msgType = "DEPRECATED_BEHAVIOR";
                warn_category = true;
            break;
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
                msgType = "UNDEFINED_BEHAVIOR";
                error_category = true;
            break;
            case GL_DEBUG_TYPE_PORTABILITY:
                msgType = "PORTABILITY";
                warn_category = true;
            break;
            case GL_DEBUG_TYPE_PERFORMANCE:
                msgType = "PERFORMANCE";
                warn_category = true;
            break;
            case GL_DEBUG_TYPE_MARKER:
                msgType = "MARKER";
            break;
            case GL_DEBUG_TYPE_PUSH_GROUP:
                msgType = "PSUH_GROUP";
            break;
            case GL_DEBUG_TYPE_POP_GROUP:
                msgType = "POP_GROUP";
            break;
            case GL_DEBUG_TYPE_OTHER:
                msgType = "OTHER";
            break;
        }

        if (error_category)
        {
            CM_CORE_LOG_ERROR("[OpenGL:{}]: {}", msgType, message);
        }
        else if (warn_category)
        {
            CM_CORE_LOG_WARN("[OpenGL:{}]: {}", msgType, message);
        }
        else
        {
            CM_CORE_LOG_TRACE("[OpenGL:{}]: {}", msgType, message);
        }
    }

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

        // Setup OpenGL Debug Messages
        glDebugMessageCallback(glDebugCallback, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
        glDebugMessageControl(GL_DONT_CARE, GL_DEBUG_TYPE_OTHER, GL_DONT_CARE, 0, nullptr, GL_FALSE);
        glEnable(GL_DEBUG_OUTPUT);

        // Enable OpenGL Flags
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        // glFrontFace(GL_CW);

        // Deactivate VSync (slowness of window motion on Linux when VSync is on)
        m_window->setVSync(false);

        // set Event callback
        m_window->setEventCallback(BIND_METHOD(Application::onEvent));

        m_isInitialized = true;
        CM_CORE_LOG_DEBUG("Finished initializing the application");
    }

    void Application::onStart() {}
    void Application::onUpdate(double deltaTime) {}
    void Application::onFixedUpdate(float fixedDeltaTime) {}
    void Application::onRender() {}

    void Application::onEvent(Event& e)
    {
        // CM_CORE_LOG_DEBUG("Application::onEvent - Event received: {}", e.toString());
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
    }

    bool Application::onWindowResized(WindowResizedEvent& e) {}
    bool Application::onWindowLostFocus(WindowLostFocusEvent& e) {}
    bool Application::onWindowGainedFocus(WindowGainedFocusEvent& e) {}
    bool Application::onKeyPressed(KeyPressedEvent& e) {}
    bool Application::onKeyReleased(KeyReleasedEvent& e) {}
    bool Application::onTextEntered(KeyTextEnteredEvent& e) {}
    bool Application::onMouseMoved(MouseMovedEvent& e) {}
    bool Application::onMouseEnteredWindow(MouseEnteredWindowEvent& e) {}
    bool Application::onMouseLeftWindow(MouseLeftWindowEvent& e) {}
    bool Application::onVerticalMouseWheelScrolled(VerticalMouseWheelScrolledEvent& e) {}
    bool Application::onHorizontalMouseWheelScrolled(HorizontalMouseWheelScrolledEvent& e) {}
    bool Application::onMouseButtonPressed(MouseButtonPressedEvent& e) {}
    bool Application::onMouseButtonRelease(MouseButtonReleasedEvent& e) {}

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
        auto previousTime = steady_clock::now();
        auto currentTime = previousTime;
        duration<double, std::nano> timeSinceLastFPSDisplay;
        duration<double, std::nano> elapsedTime;
        unsigned int frameCounter = 0;
        duration<float, std::milli> fpsCapTime(0.0f);
        double deltaTime(0.0f);

        if (m_fpsCap)
        {
            fpsCapTime = duration<float, std::milli>(1000.0f / m_fpsCap);
            CM_CORE_LOG_DEBUG("FPS Cap Time set to: {}ms", fpsCapTime.count());
        }

        while(!m_window->isCloseRequested())
        {
            currentTime = steady_clock::now();
            elapsedTime = currentTime - previousTime;
            previousTime = currentTime;
            lag += elapsedTime;

            // poll window events
            m_window->pollEvent();

            // Update as lag permits
            while(lag >= fixedUpdateTime)
            {
                // CM_CORE_LOG_DEBUG("calling onFixedUpda`te function. lag: {}", lag.count());
                onFixedUpdate(fixedUpdateTime.count());
                lag -= fixedUpdateTime;
            }

            deltaTime = duration_cast<duration<double, std::milli>>(elapsedTime).count();
            onUpdate(deltaTime);

            // Compute and display FPS (every 2s)
            frameCounter++;
            timeSinceLastFPSDisplay += elapsedTime;
            if (timeSinceLastFPSDisplay > seconds(2))
            {
                float fps = frameCounter / (duration_cast<seconds>(timeSinceLastFPSDisplay).count());
                frameCounter = 0;
                timeSinceLastFPSDisplay = milliseconds::zero();
                CM_CORE_LOG_DEBUG("FPS: {}", fps);
            }

            m_window->clearBuffers();

            onRender();

            m_window->swapBuffers();

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
                std::this_thread::sleep_for(milliseconds(1));
            }
        }
        m_window->close();
        CM_CORE_LOG_DEBUG("Exit main loop");
    }

} // namespace comet