#pragma once

#include <comet/event.h>
#include <glm/vec3.hpp>
#include <string>
#include <functional>
#include <vector>

namespace comet
{

    struct WindowSpec
    {
        WindowSpec(const std::string& pTitle = "(@)Comet Engine", unsigned int pWidth = 1920, unsigned int pHeight = 1080)
            : title(pTitle), width(pWidth), height(pHeight) {}

        std::string title;
        uint16_t width;
        uint16_t height;
        glm::vec3 backgroundColor{0.0f, 0.01f, 0.02f};
        uint8_t depthBufferBits{24};
    };

    // Window Interface (implemetation is delegated to platform specific classes)
    class Window
    {
    public:
        using EventCallbackFn = std::function<void(Event&)>;

        Window(const WindowSpec& spec = WindowSpec()) : m_windowSpec(spec) {}
        virtual ~Window() {}

        const WindowSpec& getWindowSpec() const { return m_windowSpec; }

        void addEventCallback(const EventCallbackFn& callback) { m_eventCallbacks.emplace_back(callback); }
        void dispatchEvent(Event& e)
        {
            for (auto& callbackFn : m_eventCallbacks)
            {
                callbackFn(e);
            }
        }

        virtual void swapBuffers() const = 0;
        virtual void clearBuffers() const = 0;
        virtual unsigned int getWidth() const = 0;
        virtual unsigned int getHeight() const = 0;
        virtual void setVSync(bool enabled) = 0;
        virtual bool isVSync() const = 0;
        virtual bool isClosed() const = 0;
        virtual void close() = 0;
        virtual bool isCloseRequested() const = 0;
        virtual void closeRequested() = 0;
        virtual void pollEvent() = 0;

        virtual void* getPlatformWindow() const = 0;

        // Factory
        static Window* create(const WindowSpec& spec = WindowSpec());

    protected:
        std::vector<EventCallbackFn> m_eventCallbacks;
        WindowSpec m_windowSpec;
    };

} // namespace comet
