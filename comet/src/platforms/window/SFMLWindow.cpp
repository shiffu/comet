#include "SFMLWindow.h"
#include <SFML/Window.hpp>
#include <comet/log.h>

namespace comet
{
    // Factory
    Window* SFMLWindow::create(const WindowSpec& spec)
    {
        return new SFMLWindow(spec);
    }

    SFMLWindow::SFMLWindow(const WindowSpec& spec)
    {
        m_sfWindow = new sf::Window(sf::VideoMode(spec.width, spec.height), spec.title);
    }

    SFMLWindow::~SFMLWindow()
    {
        delete m_sfWindow;
        m_sfWindow = nullptr;
    }
 
    void SFMLWindow::pollEvent() const
    {
        sf::Event event;

        while (m_sfWindow->pollEvent(event))
        {
            // check the type of the event...
            switch (event.type)
            {
                // window closed
                case sf::Event::Closed:
                    m_sfWindow->close();
                    break;

                // key pressed
                case sf::Event::KeyPressed:
                    CM_LOG_INFO("Key pressed: {}", event.key.code);
                    break;

                // we don't process other types of events for now
                // TODO: Implement proper Keyboard and Mouse events management
                default:
                    break;
            }
        }
    }

    void SFMLWindow::onUpdate()
    {
        m_sfWindow->display();
    }

    // Window info
    unsigned int SFMLWindow::getWidth() const
    {
        return m_sfWindow->getSize().x;
    }

    unsigned int SFMLWindow::getHeight() const
    {
        return m_sfWindow->getSize().y;
    }

    void SFMLWindow::setVSync(bool enabled)
    {
        m_isVSync = enabled;
        m_sfWindow->setVerticalSyncEnabled(m_isVSync);
    }

    bool SFMLWindow::isVSync() const
    {
        return m_isVSync;
    }

    bool SFMLWindow::isClosed() const
    {
        return !m_sfWindow->isOpen();
    }

} // namespace comet
