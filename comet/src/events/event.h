namespace comet
{
    enum class EventType
    {
        None = 0,
        WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
        MouseMove, MouseScrolled, MouseButtonPressed, MouseButtonReleased,
        KeyPressed, KeyReleased
    };

    class Event
    {
    public:
        virtual EventType getEventType() const = 0;
    protected:
        bool m_handled = false;
    };

} // namespace comet
