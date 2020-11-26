#pragma once

#include <comet/window.h>

namespace comet
{
    
    class ImguiWrapper
    {
    public:
        static ImguiWrapper* create();

        virtual ~ImguiWrapper() {};

        virtual void init(const Window* window) = 0;
        virtual void newFrame() = 0;
        virtual void render() = 0;
        virtual void shutdown() = 0;
        
    private:
        ImguiWrapper* s_imguiWrapperImp;
    };

} // namespace comet