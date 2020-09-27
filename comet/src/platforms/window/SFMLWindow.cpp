#include <iostream>
#include <sstream>
#include <string>
#include <glad/glad.h>
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
        if(gladLoadGL())
        {
            std::cout << "Glad loaded OpenGL with no context!" << std::endl;
            exit(-1);
        } 
        
        m_sfWindow = new sf::Window(sf::VideoMode(spec.width, spec.height), "");
        std::stringstream ss;
        ss << spec.title << " (" << (const unsigned char*)glGetString(GL_VERSION) << ")";
        m_sfWindow->setTitle(ss.str());

        m_sfWindow->setVerticalSyncEnabled(true);

        // activate the window
        m_sfWindow->setActive(true);

        if(!gladLoadGL())
        {
            std::cout << "Glad failed to initialize!" << std::endl;
            exit(-1);
        }

        CM_CORE_LOG_INFO("Graphic card: {}", glGetString(GL_RENDERER));
        CM_CORE_LOG_INFO("GL Version: {}", glGetString(GL_VERSION));
        CM_CORE_LOG_INFO("GLSL: {}", glGetString(GL_SHADING_LANGUAGE_VERSION));
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

                // adjust the viewport when the window is resized
                case sf::Event::Resized:
                    CM_LOG_INFO("Window resized: {} {}", event.size.width, event.size.height);
                    glViewport(0, 0, event.size.width, event.size.height);
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
    }

    void SFMLWindow::swapBuffers() const
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
