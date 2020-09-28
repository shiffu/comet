#pragma once

#include <string>

namespace comet
{

    struct WindowSpec
    {
        WindowSpec(const std::string& pTitle = "(@)Comet Engine", unsigned int pWidth = 1280, unsigned int pHeight = 720)
            : title(pTitle), width(pWidth), height(pHeight) {}

        std::string title;
        unsigned int width;
        unsigned int height;
    };

    // Window Interface (implemetation is delegated to platform specific classes)
    class Window
    {
    public:
        virtual ~Window() {}

        virtual void swapBuffers() const = 0;
        virtual unsigned int getWidth() const = 0;
        virtual unsigned int getHeight() const = 0;
        virtual void setVSync(bool enabled) = 0;
        virtual bool isVSync() const = 0;
        virtual bool isClosed() const = 0;
        virtual void pollEvent() const = 0;

        // Factory
        static Window* create(const WindowSpec& spec = WindowSpec());
    };

} // namespace comet
