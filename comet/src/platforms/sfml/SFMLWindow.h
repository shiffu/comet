#pragma once

#include <comet/window.h>

namespace sf {
    class Window;
}

namespace comet
{

    class SFMLWindow : public Window
    {
    public:
        SFMLWindow(const WindowSpec& spec);
        virtual ~SFMLWindow();

        virtual void pollEvent() const override;
        virtual void swapBuffers() const override;

        // Window info
        virtual unsigned int getWidth() const override;
        virtual unsigned int getHeight() const override;
        virtual void setVSync(bool enabled) override;
        virtual bool isVSync() const override;
        virtual bool isClosed() const override;

        virtual void* getPlatformWindow() const override;

        // Factory
        static Window* create(const WindowSpec& spec = WindowSpec());
        
    private:
        sf::Window* m_sfWindow = nullptr;
        // vsync is disable by default in sfml (cf documentation)
        bool m_isVSync = false;
    };
}