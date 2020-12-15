#include "imguiWrapper.h"
#include <comet/platform.h>

#include <platforms/glfw/imguiGlfwWrapper.h>

namespace comet
{
    
    std::unique_ptr<ImguiWrapper> ImguiWrapper::create()
    {
        std::unique_ptr<ImguiWrapper> instance{};

        #if COMET_WINDOW_IMPL == COMET_WINDOW_IMPL_GLFW
            instance = std::make_unique<ImguiGlfwWrapper>();
        #elif COMET_WINDOW_IMPL == COMET_WINDOW_IMPL_SFML
            #warning "ImGui Wrapper for SFML Window management is not supported for now"
        #else
            #warning "No ImGui Wrapper available for this plaftorm"
        #endif

        return instance;
    }

} // namespace comet