#pragma once

#include <comet/input.h>
#include <string>
#include <sstream>
#include <functional>
#include <utility>

namespace comet
{
    enum class EventType
    {
        None = 0,
        WindowClosed, WindowResized, WindowGainedFocus, WindowLostFocus, WindowMoved,
        MouseMoved, MouseEnteredWindow, MouseLeftWindow, VerticalMouseScrolled, HorizontalMouseScrolled, MouseButtonPressed, MouseButtonReleased,
        KeyPressed, KeyReleased, TextEntered
    };

    class Event
    {
        friend class EventDispatcher;
    public:
        virtual EventType getEventType() const = 0;
        virtual const char* getName() const = 0;
        virtual std::string toString() const { return getName(); }

    protected:
        bool m_handled = false;
    };

    inline std::ostream& operator<<(std::ostream& os, const Event& e)
    {
        return os << e.toString();
    }

    class EventDispatcher
    {
        public:
            EventDispatcher(Event& e) : m_event(e) {}

            template<typename T>
            bool dispatch(std::function<bool(T&)> func)
            {
                if (m_event.getEventType() == T::getStaticType())
                {
                    m_event.m_handled = func(*(T*)&m_event);
                    return true;
                }

                return false;
            }

        private:
            Event& m_event;
    };

    #define EVENT_CLASS_TYPE_DEF(type) \
            static EventType getStaticType() { return EventType::type ; }\
            virtual EventType getEventType() const override { return getStaticType(); }\
            virtual const char* getName() const override { return #type; }
    

    // All Event implementation classes
    // ================================

    // Application events
    class WindowResizedEvent : public Event
    {
        public:
            WindowResizedEvent(unsigned int width, unsigned int height) : m_width(width), m_height(height) {}
            EVENT_CLASS_TYPE_DEF(WindowResized);

            unsigned int getWidth() const { return m_width; }
            unsigned int getHeight() const { return m_height; }
            std::string toString() const override
            {
                std::stringstream ss;
                ss << "WindowResizedEvent (" << m_width << ", " << m_height << ")";
                return ss.str();
            }

        private:
            unsigned int m_width;
            unsigned int m_height;
    };

    class WindowLostFocusEvent : public Event
    {
        public:
            WindowLostFocusEvent() {}
            EVENT_CLASS_TYPE_DEF(WindowLostFocus);
    };

    class WindowGainedFocusEvent : public Event
    {
        public:
            WindowGainedFocusEvent() {}
            EVENT_CLASS_TYPE_DEF(WindowGainedFocus);
    };

    // Key events
    class KeyEvent : public Event
    {
        public:
            Input::Key getKeyCode() const { return m_keyCode; }

        protected:
            KeyEvent(Input::Key keycode) : m_keyCode(keycode) {}

        protected:
            Input::Key m_keyCode;
    };

    class KeyPressedEvent : public KeyEvent
    {
        public:
            KeyPressedEvent(Input::Key keycode, int repeat) : KeyEvent(keycode), m_repeat(repeat) {}
            EVENT_CLASS_TYPE_DEF(KeyPressed);

            int getRepeat() const { return m_repeat; }
            std::string toString() const override
            {
                std::stringstream ss;
                ss << "KeyPressedEvent: " << m_keyCode << " (" << m_repeat << " repeat(s))";
                return ss.str();
            }

        private:
            int m_repeat;
    };

    class KeyReleasedEvent : public KeyEvent
    {
        public:
            KeyReleasedEvent(Input::Key keycode) : KeyEvent(keycode) {}
            EVENT_CLASS_TYPE_DEF(KeyReleased);

            int getRepeat() const { return m_repeat; }
            std::string toString() const override
            {
                std::stringstream ss;
                ss << "KeyReleasedEvent: " << m_keyCode << " (" << m_repeat << " repeat(s))";
                return ss.str();
            }

        private:
            int m_repeat;
    };

    class KeyTextEnteredEvent : public Event
    {
        public:
            KeyTextEnteredEvent(unsigned int unicode) : m_unicode(unicode) {}
            EVENT_CLASS_TYPE_DEF(TextEntered);

            unsigned int getUnicode() const { return m_unicode; }
            std::string toString() const override
            {
                std::stringstream ss;
                ss << "KeyTextEnteredEvent: " << m_unicode;
                return ss.str();
            }

        private:
            unsigned int m_unicode;
    };

    // Mouse events
    class MouseMovedEvent : public Event
    {
        public:
            MouseMovedEvent(int x, int y) : m_x(x), m_y(y) {}
            EVENT_CLASS_TYPE_DEF(MouseMoved);

            std::pair<int, int> getPosition() const { return std::make_pair(m_x, m_y); }
            std::string toString() const override
            {
                std::stringstream ss;
                ss << "MouseMovedEvent (" << m_x << ", " << m_y << ")";
                return ss.str();
            }

        private:
            int m_x;
            int m_y;
    };

    class MouseEnteredWindowEvent : public Event
    {
        public:
            MouseEnteredWindowEvent() {}
            EVENT_CLASS_TYPE_DEF(MouseEnteredWindow);
    };

    class MouseLeftWindowEvent : public Event
    {
        public:
            MouseLeftWindowEvent() {}
            EVENT_CLASS_TYPE_DEF(MouseLeftWindow);
    };

    class MouseWheelScrolledEvent : public Event
    {
        public:
            MouseWheelScrolledEvent(int delta) : m_delta(delta) {}

            int getDelta() const { return m_delta; }
        
        private:
            int m_delta;
    };

    class VerticalMouseWheelScrolledEvent : public MouseWheelScrolledEvent
    {
        public:
            VerticalMouseWheelScrolledEvent(int delta) : MouseWheelScrolledEvent(delta) {}
            EVENT_CLASS_TYPE_DEF(VerticalMouseScrolled);

            std::string toString() const override
            {
                std::stringstream ss;
                ss << "VerticalMouseWheelScrolledEvent (" << getDelta() << ")";
                return ss.str();
            }
    };

    class HorizontalMouseWheelScrolledEvent : public MouseWheelScrolledEvent
    {
        public:
            HorizontalMouseWheelScrolledEvent(int delta) : MouseWheelScrolledEvent(delta) {}
            EVENT_CLASS_TYPE_DEF(HorizontalMouseScrolled);

            std::string toString() const override
            {
                std::stringstream ss;
                ss << "HorizontalMouseWheelScrolledEvent (" << getDelta() << ")";
                return ss.str();
            }
    };

    class MouseButtonEvent : public Event
    {
        public:
            MouseButtonEvent(Input::MouseButton button, int x, int y) : m_button(button), m_x(x), m_y(y) {}

            Input::MouseButton getButton() const { return m_button; }
            std::pair<int, int> getPosition() const { return std::make_pair(m_x, m_y); }
        
        private:
            Input::MouseButton m_button;
            int m_x;
            int m_y;
    };

    class MouseButtonPressedEvent : public MouseButtonEvent
    {
        public:
            MouseButtonPressedEvent(Input::MouseButton button, int x, int y) : MouseButtonEvent(button, x, y) {}
            EVENT_CLASS_TYPE_DEF(MouseButtonPressed);

            std::string toString() const override
            {
                std::stringstream ss;
                auto [x, y] = getPosition();
                ss << "MouseButtonPressedEvent " << getButton() << " (" << x  << ", " << y << ")";
                return ss.str();
            }
    };

    class MouseButtonReleasedEvent : public MouseButtonEvent
    {
        public:
            MouseButtonReleasedEvent(Input::MouseButton button, int x, int y) : MouseButtonEvent(button, x, y) {}
            EVENT_CLASS_TYPE_DEF(MouseButtonReleased);

            std::string toString() const override
            {
                std::stringstream ss;
                auto [x, y] = getPosition();
                ss << "MouseButtonPressedEvent " << getButton() << " (" << x  << ", " << y << ")";
                return ss.str();
            }
    };

} // namespace comet
