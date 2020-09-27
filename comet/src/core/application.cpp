#include <glad/glad.h>
#include <comet/log.h>
#include <comet/application.h>
#include <comet/window.h>
#include <platforms/window/SFMLWindow.h>

namespace comet
{

    Application::Application(const WindowSpec& spec)
    {
        init(spec);
        CM_CORE_LOG_DEBUG("Application contructor");
    }

    Application::~Application()
    {
        CM_CORE_LOG_DEBUG("Application destructor");
    };

    void Application::init(const WindowSpec& spec)
    {
        Log::init();
        CM_CORE_LOG_DEBUG("Initializing the application");
        m_window = Window::create(spec);
        m_isInitialized = true;
        CM_CORE_LOG_DEBUG("Finished initializing the application");
    }

    void Application::onStart()
    {
    }

    void Application::run()
    {
        CM_CORE_LOG_INFO("Start running the application");
        m_isRunning = true;

        CM_CORE_LOG_DEBUG("Calling onStart()");
        onStart();

        CM_CORE_LOG_DEBUG("Starting main loop");
        while(!m_window->isClosed())
        {
            //TODO: Move this code in the renderer (probably)
            glClearDepth(1.0f);
            glClearColor(0.9f, 0.5f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            m_window->pollEvent();
            onUpdate();
            m_window->swapBuffers();
        }
        CM_CORE_LOG_DEBUG("Exit main loop");
    }

} // namespace comet