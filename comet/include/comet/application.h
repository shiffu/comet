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
        virtual void onUpdate(double deltaTime);
        virtual void onFixedUpdate(float fixedDeltaTime);
        virtual void onRender();
        
        Window& getWindow() const { return *m_window; }

        // Configuration
        void setFPSCap(unsigned int fpsCap);

    private:
        bool m_isRunning = false;
        bool m_isInitialized = false;
        Window* m_window = nullptr;
        unsigned int m_fpsCap{0};
        // Fixed update time in ms (used to onFixedUpdate function call)
        float m_fixedUpdateTime{20.0f};

        void init(const WindowSpec& spec);
    };

} // namespace comet

