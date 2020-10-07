#pragma once

#include <unordered_map>
#include <comet/input.h>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

namespace comet
{
    
    class SFMLInput : public Input
    {
    public:
        SFMLInput() {}

        inline static sf::Keyboard::Key cometKeyToSFMLKey(Key key) { return m_cometKeyToSFMLKeyMap[key]; }
        inline static Key sfmlKeyToCometKey(sf::Keyboard::Key key) { return m_SFMLKeyToCometKeyMap[key]; }

    protected:
        bool isKeyPressedImpl(Key key);
        bool isMouseButtonPressedImpl(Input::MouseButton button);
        std::pair<int, int> getMousePositionImpl();

    private:
        static std::unordered_map<Key, sf::Keyboard::Key> m_cometKeyToSFMLKeyMap;
        static std::unordered_map<sf::Keyboard::Key, Key> m_SFMLKeyToCometKeyMap;
    };

} // namespace comet
