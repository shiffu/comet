#pragma once

#include <rendering/imguiWrapper.h>

namespace comet
{

    class ImguiGlfwWrapper : public ImguiWrapper
    {
    public:
        ~ImguiGlfwWrapper() {};

        void init(const Window* window) override;
        void newFrame() override;
        void render() override;
        void shutdown() override;
    };
    
} // namespace comet
