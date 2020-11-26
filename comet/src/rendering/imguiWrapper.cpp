#include "imguiWrapper.h"
#include <platforms/platform.h>

#include <platforms/glfw/imguiGlfwWrapper.h>

namespace comet
{
    
    ImguiWrapper* ImguiWrapper::create()
    {
        ImguiWrapper* instance{nullptr};

        #if COMET_WINDOW_IMPL == COMET_WINDOW_IMPL_GLFW
            instance = new ImguiGlfwWrapper();
        #elif COMET_WINDOW_IMPL == COMET_WINDOW_IMPL_SFML
            #warning "ImGui Wrapper for SFML Window management is not supported for now"
        #else
            #warning "No ImGui Wrapper available for this plaftorm"
        #endif

        return instance;
    }

} // namespace comet