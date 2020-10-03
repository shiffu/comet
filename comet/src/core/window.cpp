#include <comet/window.h>
#include <platforms/sfml/SFMLWindow.h>

namespace comet
{
    Window* Window::create(const WindowSpec& spec)
    {
        return new SFMLWindow(spec);
    }

} // namespace comet
