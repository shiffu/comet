#include <glad/glad.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <comet/log.h>
#include <comet/application.h>
#include <comet/window.h>
#include <platforms/sfml/SFMLWindow.h>

namespace comet
{

    static void APIENTRY glDebugCallback(GLenum source,
                                        GLenum type,
                                        GLuint id,
                                        GLenum severity,
                                        GLsizei length,
                                        const GLchar* message,
                                        const void* userParam)
    {
        CM_CORE_LOG_DEBUG("OpenGL Error: {}", message);
        exit(EXIT_FAILURE);
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
        glDebugMessageCallback(glDebugCallback, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
        m_isInitialized = true;
        CM_CORE_LOG_DEBUG("Finished initializing the application");
    }

    void Application::onStart() {}
    void Application::onUpdate(double deltaTime) {}
    void Application::onFixedUpdate(float fixedDeltaTime) {}
    void Application::onRender() {}

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

        while(!m_window->isClosed())
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

            //TODO: Move this code in the renderer (probably)
            glClearDepth(1.0f);
            glClearColor(0.0f, 0.1f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
        CM_CORE_LOG_DEBUG("Exit main loop");
    }

} // namespace comet