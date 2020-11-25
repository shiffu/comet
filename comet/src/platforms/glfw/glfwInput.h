#pragma once

#include <comet/input.h>
#include <unordered_map>

namespace comet
{
    class GLFWInput : public Input
    {
    public:
        GLFWInput() {}

        inline static int cometKeyToSFMLKey(Key key) { return m_cometKeyToGLFWKeyMap[key]; }
        inline static Key glfwKeyToCometKey(int key) { return m_GLFWKeyToCometKeyMap[key]; }

        inline static int cometButtonToSFMLButton(MouseButton Button) { return m_cometButtonToGLFWButtonMap[Button]; }
        inline static MouseButton glfwButtonToCometButton(int Button) { return m_GLFWButtonToCometButtonMap[Button]; }

    protected:
        bool isKeyPressedImpl(Key key);
        bool isMouseButtonPressedImpl(Input::MouseButton button);
        std::pair<int, int> getMousePositionImpl();

    private:
        static std::unordered_map<Key, int> m_cometKeyToGLFWKeyMap;
        static std::unordered_map<int, Key> m_GLFWKeyToCometKeyMap;
        static std::unordered_map<MouseButton, int> m_cometButtonToGLFWButtonMap;
        static std::unordered_map<int, MouseButton> m_GLFWButtonToCometButtonMap;
    };

} // namespace comet