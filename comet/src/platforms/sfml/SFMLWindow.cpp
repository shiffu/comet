#include <glad/glad.h>
#include "SFMLWindow.h"
#include <SFML/Window.hpp>
#include "SFMLInput.h"
#include <comet/event.h>
#include <comet/log.h>
#include <glm/vec4.hpp>
#include <iostream>
#include <sstream>
#include <string>

namespace comet
{

    // Factory
    Window* SFMLWindow::create(const WindowSpec& spec)
    {
        CM_CORE_LOG_DEBUG("Using SFML Window Implementation");
        return new SFMLWindow(spec);
    }

    SFMLWindow::SFMLWindow(const WindowSpec& spec) : Window(spec)
    {
        if(gladLoadGL())
        {
            CM_CORE_LOG_FATAL("Glad loaded OpenGL with no context");
            exit(EXIT_FAILURE);
        } 
        
        sf::ContextSettings windowSettings;
        windowSettings.depthBits = spec.depthBufferBits;

        m_sfWindow = new sf::Window(sf::VideoMode(spec.width, spec.height), "",
                        sf::Style::Resize | sf::Style::Close, windowSettings);

        // We need to set the title after window creation since we call glGetString, thus
        // we need the OpenGL Context to be created
        std::stringstream ss;
        ss << spec.title << " (" << (const unsigned char*)glGetString(GL_VERSION) << ")";
        m_sfWindow->setTitle(ss.str());

        // TODO: Figure out why VSync is slowing down window movement responsiveness on Linux
        // Somehow this slows down a lot the responsiveness of the window on Linux (when moving the window)
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

    void SFMLWindow::pollEvent()
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
                    closeRequested();
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

    void SFMLWindow::clearBuffers() const
    {
        glClearColor(
            m_windowSpec.backgroundColor.r,
            m_windowSpec.backgroundColor.g,
            m_windowSpec.backgroundColor.b, 1.0f);

        if (m_windowSpec.depthBufferBits > 0)
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
        else
        {
            glClear(GL_COLOR_BUFFER_BIT);
        }
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

    bool SFMLWindow::isCloseRequested() const
    {
        return m_isCloseRequested;
    }

    void SFMLWindow::closeRequested()
    {
        m_isCloseRequested = true;
    }

    void SFMLWindow::close()
    {
        m_sfWindow->close();
    }

    bool SFMLWindow::isClosed() const
    {
        return !m_sfWindow->isOpen();
    }

} // namespace comet
