#include <glad/glad.h>
#include <chrono>
#include <thread>
#include <comet/log.h>
#include <comet/application.h>
#include <comet/window.h>
#include <platforms/window/SFMLWindow.h>

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
    void Application::onUpdate() {}
    void Application::onRender(float deltaTime) {}

    void Application::run()
    {
        CM_CORE_LOG_INFO("Start running the application");
        m_isRunning = true;

        CM_CORE_LOG_DEBUG("Calling onStart()");
        onStart();

        CM_CORE_LOG_DEBUG("Starting main loop");
        using namespace std::chrono;

        nanoseconds lag(0ns);
        nanoseconds updateTime(30ms);
        auto previousTime = steady_clock::now();
        auto currentTime = steady_clock::now();
        steady_clock::duration timeSinceLastFPSDisplay;
        steady_clock::duration elapsedTime;
        unsigned int frameCounter = 0;
        float fpsCapTime;
        if (m_fpsCap)
        {
            fpsCapTime = 1000.0f / m_fpsCap;
        }

        while(!m_window->isClosed())
        {
            currentTime = steady_clock::now();
            elapsedTime = currentTime - previousTime;
            previousTime = currentTime;
            lag += duration_cast<nanoseconds>(elapsedTime);

            // poll window events
            m_window->pollEvent();

            // Update as lag permits
            while(lag >= updateTime)
            {
                onUpdate();
                lag -= updateTime;
            }

            // Compute and display FPS (every 3s)
            frameCounter++;
            timeSinceLastFPSDisplay += elapsedTime;
            if (timeSinceLastFPSDisplay > seconds(3))
            {
                float fps = frameCounter / (duration_cast<milliseconds>(timeSinceLastFPSDisplay).count() / 1000.0f);
                frameCounter = 0;
                timeSinceLastFPSDisplay = milliseconds::zero();
                CM_CORE_LOG_DEBUG("FPS: {}", fps);
            }

            //TODO: Move this code in the renderer (probably)
            glClearDepth(1.0f);
            glClearColor(0.0f, 0.1f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            onRender(duration_cast<milliseconds>(elapsedTime).count());

            m_window->swapBuffers();

            // Is FPS Cap needed?
            if (m_fpsCap)
            {
                auto waitTime = static_cast<int>(fpsCapTime - duration_cast<milliseconds>(elapsedTime).count());
                if (waitTime > 1)
                {
                    std::this_thread::sleep_for(milliseconds(waitTime));
                }
            }
        }
        CM_CORE_LOG_DEBUG("Exit main loop");
    }

} // namespace comet