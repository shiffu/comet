#include <glad/glad.h>
#include <iostream>
#include <sstream>
#include <string>
#include "SFMLWindow.h"
#include <SFML/Window.hpp>
#include "SFMLInput.h"
#include <comet/event.h>

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
            CM_CORE_LOG_FATAL("Glad loaded OpenGL with no context");
            exit(EXIT_FAILURE);
        } 
        
        m_sfWindow = new sf::Window(sf::VideoMode(spec.width, spec.height), "");
        std::stringstream ss;
        ss << spec.title << " (" << (const unsigned char*)glGetString(GL_VERSION) << ")";
        m_sfWindow->setTitle(ss.str());

        // Somehow this slows down a lot the responsiveness of the window on Linux (when moving the window)
        // TODO: Figure out why VSync is slowing down window movement responsiveness on Linux
        // m_sfWindow->setVerticalSyncEnabled(true);

        // activate the window
        m_sfWindow->setActive(true);

        if(!gladLoadGL())
        {
            CM_CORE_LOG_FATAL("Glad failed to initialize");
            exit(EXIT_FAILURE);
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

    void* SFMLWindow::getPlatformWindow() const
    {
        return m_sfWindow;
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
                {
                    m_sfWindow->close();
                    break;
                }

                // adjust the viewport when the window is resized
                case sf::Event::Resized:
                {
                    glViewport(0, 0, event.size.width, event.size.height);
                    WindowResizedEvent cometEvent(event.size.width, event.size.height);
                    m_eventCallbackFn(cometEvent);
                    break;
                }

                case sf::Event::LostFocus:
                {
                    WindowLostFocusEvent cometEvent;
                    m_eventCallbackFn(cometEvent);
                    break;
                }

                case sf::Event::GainedFocus:
                {
                    WindowGainedFocusEvent cometEvent;
                    m_eventCallbackFn(cometEvent);
                    break;
                }

                case sf::Event::KeyPressed:
                {
                    KeyPressedEvent cometEvent(SFMLInput::sfmlKeyToCometKey(event.key.code), 0);
                    m_eventCallbackFn(cometEvent);
                    break;
                }

                case sf::Event::KeyReleased:
                {
                    KeyReleasedEvent cometEvent(SFMLInput::sfmlKeyToCometKey(event.key.code));
                    m_eventCallbackFn(cometEvent);
                    break;
                }

                case sf::Event::TextEntered:
                {
                    KeyTextEnteredEvent cometEvent(event.text.unicode);
                    m_eventCallbackFn(cometEvent);
                    break;
                }

                case sf::Event::MouseMoved:
                {
                    MouseMovedEvent cometEvent(event.mouseMove.x, event.mouseMove.y);
                    m_eventCallbackFn(cometEvent);
                    break;
                }

                case sf::Event::MouseEntered:
                {
                    MouseEnteredWindowEvent cometEvent;
                    m_eventCallbackFn(cometEvent);
                    break;
                }

                case sf::Event::MouseLeft:
                {
                    MouseLeftWindowEvent cometEvent;
                    m_eventCallbackFn(cometEvent);
                    break;
                }

                case sf::Event::MouseWheelScrolled:
                {
                    if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel)
                    {
                        VerticalMouseWheelScrolledEvent cometEvent(event.mouseWheelScroll.delta);
                        m_eventCallbackFn(cometEvent);
                    }
                    else if (event.mouseWheelScroll.wheel == sf::Mouse::HorizontalWheel)
                    {
                        HorizontalMouseWheelScrolledEvent cometEvent(event.mouseWheelScroll.delta);
                        m_eventCallbackFn(cometEvent);
                    }
                    break;
                }

                case sf::Event::MouseButtonPressed:
                {
                    MouseButtonPressedEvent cometEvent((Input::MouseButton)event.mouseButton.button, event.mouseButton.x, event.mouseButton.y);
                    m_eventCallbackFn(cometEvent);
                    break;
                }

                case sf::Event::MouseButtonReleased:
                {
                    MouseButtonReleasedEvent cometEvent((Input::MouseButton)event.mouseButton.button, event.mouseButton.x, event.mouseButton.y);
                    m_eventCallbackFn(cometEvent);
                    break;
                }

                // we don't process other types of events for now
                // TODO: implement Joystick events
                default:
                    break;
            }
        }
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
