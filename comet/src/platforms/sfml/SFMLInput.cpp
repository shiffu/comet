#include "SFMLInput.h"
#include <comet/application.h>
#include <utility>

namespace comet
{

    Input* Input::m_instance = new SFMLInput();


    bool SFMLInput::isKeyPressedImpl(Key key)
    {
        return sf::Keyboard::isKeyPressed(m_cometKeyToSFMLKeyMap[key]);
    }

    bool SFMLInput::isMouseButtonPressedImpl(MouseButton button)
    {
        //TODO: Map like for the keys? Issue here is if sfml decides to change the enum values.
        sf::Mouse::isButtonPressed(sf::Mouse::Button(button));
    }

    std::pair<int, int> SFMLInput::getMousePositionImpl()
    {
        // get the local mouse position (relative to a window)
        auto sfWindow = static_cast<sf::Window*>(Application::getInstance()->getWindow().getPlatformWindow());
        sf::Vector2i pos = sf::Mouse::getPosition(*sfWindow);
        
        return std::make_pair(pos.x, pos.y);
    }

} // namespace comet