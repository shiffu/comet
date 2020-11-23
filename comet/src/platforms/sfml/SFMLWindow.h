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

        virtual void pollEvent() override;
        virtual void swapBuffers() const override;
        virtual void clearBuffers() const override;

        // Window info
        unsigned int getWidth() const override;
        unsigned int getHeight() const override;
        void setVSync(bool enabled) override;
        bool isVSync() const override;
        bool isClosed() const override;
        void close() override;
        bool isCloseRequested() const override;
        void closeRequested() override;

        virtual void* getPlatformWindow() const override;

        // Factory
        static Window* create(const WindowSpec& spec = WindowSpec());
        
    private:
        sf::Window* m_sfWindow = nullptr;
        bool m_isCloseRequested = false;
        // vsync is disable by default in sfml (cf documentation)
        bool m_isVSync = false;
    };
}