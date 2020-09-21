#include <comet/application.h>
#include <comet/window.h>
#include <platforms/window/SFMLWindow.h>

namespace comet
{

    void Application::init(const WindowSpec& spec)
    {
        CM_CORE_LOG_INFO("Initializing the application");
        m_window = Window::create(spec);
        m_isInitialized = true;
        CM_CORE_LOG_INFO("Finished initializing the application");
    }

    int Application::run()
    {
        CM_CORE_LOG_INFO("Start running the application");
        m_isRunning = true;

        CM_CORE_LOG_TRACE("Starting main loop");
        while(!m_window->isClosed())
        {
            m_window->pollEvent();
            m_window->onUpdate();
        }
        CM_CORE_LOG_TRACE("Exit main loop");
    }

} // namespace comet