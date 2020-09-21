#pragma once

#include <core/windowImpl.h>

namespace sf {
    class Window;
}

namespace comet
{

    class SFMLWindow : public WindowImpl
    {
    public:
        SFMLWindow(const WindowSpec& spec);
        virtual ~SFMLWindow();

        virtual void pollEvent() const override;
        virtual void onUpdate() override;

        // Window info
        virtual unsigned int getWidth() const override;
        virtual unsigned int getHeight() const override;
        virtual void setVSync(bool enabled) override;
        virtual bool isVSync() const override;
        virtual bool isClosed() const override;

        // Factory
        static Window* create(const WindowSpec& spec = WindowSpec());
        
    private:
        sf::Window* m_sfWindow = nullptr;
        bool m_isVSync = true;
    };
}