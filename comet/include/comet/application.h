#pragma once

#include <comet/log.h>
#include <comet/window.h>

namespace comet
{

class Application
{
    bool m_isRunning = false;
    bool m_isInitialized = false;
    Window* m_window = nullptr;

    void init(const WindowSpec& spec);
public:
    Application(const WindowSpec& spec = WindowSpec()) { CM_CORE_LOG_DEBUG("Application contructor"); init(spec); } ;
    virtual ~Application() { CM_CORE_LOG_DEBUG("Application destructor"); };

    virtual int run();
};

} // namespace comet

