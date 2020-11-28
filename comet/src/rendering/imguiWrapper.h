#pragma once

#include <comet/window.h>
#include <memory>

namespace comet
{
    
    class ImguiWrapper
    {
    public:
        static std::unique_ptr<ImguiWrapper> create();

        virtual ~ImguiWrapper() {};

        virtual void init(const Window* window) = 0;
        virtual void newFrame() = 0;
        virtual void render() = 0;
        virtual void shutdown() = 0;
    };

} // namespace comet