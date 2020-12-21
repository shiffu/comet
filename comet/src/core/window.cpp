#include <comet/window.h>
#include <platforms/sfml/SFMLWindow.h>
#include <platforms/glfw/glfwWindowWrapper.h>
#include <comet/platform.h>
#include <comet/assert.h>

namespace comet
{
    Window* Window::create(const WindowSpec& spec)
    {
        #if COMET_WINDOW_IMPL == COMET_WINDOW_IMPL_SFML
            return SFMLWindow::create(spec);
        #elif COMET_WINDOW_IMPL == COMET_WINDOW_IMPL_GLFW
            return GLFWWindowWrapper::create(spec);
        #endif

        ASSERT(false, "Unmanaged Window System!");
        return nullptr;
    }

} // namespace comet
