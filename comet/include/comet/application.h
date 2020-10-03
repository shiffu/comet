#pragma once

#include <comet/window.h>

namespace comet
{

    class Application
    {
    public:

        // TODO: Find a proper way to make Application a Singleton
        // Here we need the client to actually define this static function (not ideal)
        static Application* getInstance();

        Application(const Application&) = delete;
        Application& operator=(const Application) = delete;

        virtual void run();
        
        Window& getWindow() const { return *m_window; }

        // Configuration
        void setFPSCap(unsigned int fpsCap);

    protected:
        Application(const WindowSpec& spec = WindowSpec());
        virtual ~Application();

    private:
        void init(const WindowSpec& spec = WindowSpec());

        virtual void onStart();
        virtual void onUpdate(double deltaTime);
        virtual void onFixedUpdate(float fixedDeltaTime);
        virtual void onRender();

    private:
        bool m_isRunning = false;
        bool m_isInitialized = false;
        Window* m_window = nullptr;
        unsigned int m_fpsCap{0};
        // Fixed update time in ms (used to onFixedUpdate function call)
        float m_fixedUpdateTime{20.0f};
    };

} // namespace comet

