#include <comet/window.h>
#include <platforms/window/SFMLWindow.h>

namespace comet
{
    Window* Window::create(const WindowSpec& spec)
    {
        return new SFMLWindow(spec);
    }

} // namespace comet
