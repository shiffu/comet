#pragma once

#include <comet/window.h>

namespace comet
{

class Application
{
public:
    Application(const WindowSpec& spec = WindowSpec());
    virtual ~Application();

    virtual void run();
    virtual void onStart();
    virtual void onUpdate() = 0;
    
private:
    bool m_isRunning = false;
    bool m_isInitialized = false;
    Window* m_window = nullptr;

    void init(const WindowSpec& spec);
};

} // namespace comet

