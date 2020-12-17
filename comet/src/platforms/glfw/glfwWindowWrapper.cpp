#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glfwWindowWrapper.h"
#include "glfwInput.h"
#include <comet/event.h>
#include <comet/log.h>

namespace comet
{
    // Factory
    Window* GLFWWindowWrapper::create(const WindowSpec& spec)
    {
        CM_CORE_LOG_DEBUG("Using GLFW Window Implementation");
        Window* window = new GLFWWindowWrapper(spec);
        Input::setActiveWindow(window);

        return window;
    }

    GLFWWindowWrapper::GLFWWindowWrapper(const WindowSpec& spec) : Window(spec)
    {
        if(gladLoadGL())
        {
            CM_CORE_LOG_FATAL("Glad loaded OpenGL with no context");
            exit(EXIT_FAILURE);
        } 
        
        if (!glfwInit())
        {
            CM_CORE_LOG_FATAL("GLFW Init failed");
            exit(EXIT_FAILURE);
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        m_window = glfwCreateWindow(spec.width, spec.height, "", nullptr, nullptr);
        if (m_window == nullptr)
        {
            glfwTerminate();
            CM_CORE_LOG_FATAL("GLFW Window creation failed");
            exit(EXIT_FAILURE);
        }

        glfwSetWindowUserPointer(m_window, this);

        // Set all event callback functions
        glfwSetErrorCallback([](int error, const char* description)
        {
            CM_CORE_LOG_ERROR("GLFW Error ({}): {}", description);
        });

        glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window)
        {
            auto windowWrapper = (GLFWWindowWrapper*)glfwGetWindowUserPointer(window);

            windowWrapper->closeRequested();
        });

        glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
        {
            auto windowWrapper = (GLFWWindowWrapper*)glfwGetWindowUserPointer(window);

            glViewport(0, 0, width, height);
            WindowResizedEvent cometEvent(width, height);
            windowWrapper->dispatchEvent(cometEvent);
        });

        glfwSetWindowFocusCallback(m_window, [](GLFWwindow* window, int hasFocus)
        {
            auto windowWrapper = (GLFWWindowWrapper*)glfwGetWindowUserPointer(window);

            if (hasFocus)
            {
                WindowGainedFocusEvent cometEvent;
                windowWrapper->dispatchEvent(cometEvent);
            }
            else
            {
                WindowLostFocusEvent cometEvent;
                windowWrapper->dispatchEvent(cometEvent);
            }
        });

        glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            auto windowWrapper = (GLFWWindowWrapper*)glfwGetWindowUserPointer(window);

            switch (action)
            {
                case GLFW_PRESS:
                {
                    KeyPressedEvent cometEvent(GLFWInput::glfwKeyToCometKey(key), 0);
                    windowWrapper->dispatchEvent(cometEvent);
                    break;
                }

                case GLFW_REPEAT:
                {
                    KeyPressedEvent cometEvent(GLFWInput::glfwKeyToCometKey(key), 1);
                    windowWrapper->dispatchEvent(cometEvent);
                    break;
                }

                case GLFW_RELEASE:
                {
                    KeyReleasedEvent cometEvent(GLFWInput::glfwKeyToCometKey(key));
                    windowWrapper->dispatchEvent(cometEvent);
                    break;
                }
            }

        });

        glfwSetCharCallback(m_window, [](GLFWwindow* window, unsigned int codePoint)
        {
            auto windowWrapper = (GLFWWindowWrapper*)glfwGetWindowUserPointer(window);

            KeyTextEnteredEvent cometEvent(codePoint);
            windowWrapper->dispatchEvent(cometEvent);
        });

        glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xPos, double yPos)
        {
            auto windowWrapper = (GLFWWindowWrapper*)glfwGetWindowUserPointer(window);

            MouseMovedEvent cometEvent(xPos, yPos);
            windowWrapper->dispatchEvent(cometEvent);
        });

        glfwSetCursorEnterCallback(m_window, [](GLFWwindow* window, int entered)
        {
            auto windowWrapper = (GLFWWindowWrapper*)glfwGetWindowUserPointer(window);

            if (entered)
            {
                MouseEnteredWindowEvent cometEvent;
                windowWrapper->dispatchEvent(cometEvent);
            }
            else
            {
                MouseLeftWindowEvent cometEvent;
                windowWrapper->dispatchEvent(cometEvent);
            }
        });

        glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods)
        {
            auto windowWrapper = (GLFWWindowWrapper*)glfwGetWindowUserPointer(window);
            double xPos, yPos;
            glfwGetCursorPos(window, &xPos, &yPos);

            switch (action)
            {
                case GLFW_PRESS:
                {
                    MouseButtonPressedEvent cometEvent(GLFWInput::glfwButtonToCometButton(button), xPos, yPos);
                    windowWrapper->dispatchEvent(cometEvent);
                    break;
                }

                case GLFW_RELEASE:
                {
                    MouseButtonReleasedEvent cometEvent(GLFWInput::glfwButtonToCometButton(button), xPos, yPos);
                    windowWrapper->dispatchEvent(cometEvent);
                    break;
                }
            }

        });

        glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xOffset, double yOffset)
        {
            auto windowWrapper = (GLFWWindowWrapper*)glfwGetWindowUserPointer(window);

            if (yOffset != 0.0)
            {
                VerticalMouseWheelScrolledEvent cometEvent(yOffset);
                windowWrapper->dispatchEvent(cometEvent);
            }
            
            if (xOffset != 0.0)
            {
                HorizontalMouseWheelScrolledEvent cometEvent(xOffset);
                windowWrapper->dispatchEvent(cometEvent);
            }
        });

        // Set Current OpenGL Context
        glfwMakeContextCurrent(m_window);
        gladLoadGL();

        // We need to set the title after window creation since we call glGetString, thus
        // we need the OpenGL Context to be created
        std::stringstream ss;
        ss << spec.title << " (" << (const unsigned char*)glGetString(GL_VERSION) << ")";
        glfwSetWindowTitle(m_window, ss.str().c_str());

        if(!gladLoadGL())
        {
            CM_CORE_LOG_FATAL("Glad failed to initialize");
            exit(EXIT_FAILURE);
        }

        CM_CORE_LOG_INFO("Graphic card: {}", glGetString(GL_RENDERER));
        CM_CORE_LOG_INFO("GL Version: {}", glGetString(GL_VERSION));
        CM_CORE_LOG_INFO("GLSL: {}", glGetString(GL_SHADING_LANGUAGE_VERSION));
    }

    GLFWWindowWrapper::~GLFWWindowWrapper()
    {
        glfwDestroyWindow(m_window);
    }

    void* GLFWWindowWrapper::getPlatformWindow() const
    {
        return m_window;
    }

    void GLFWWindowWrapper::pollEvent()
    {
        glfwPollEvents();
    }

    void GLFWWindowWrapper::swapBuffers() const
    {
        glfwSwapBuffers(m_window);
    }

    void GLFWWindowWrapper::clearBuffers() const
    {
        // TODO: This code is actually pure OpenGL Code and as such should be in the GraphicsAPI instead
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
    unsigned int GLFWWindowWrapper::getWidth() const
    {
        int width, height;
        glfwGetWindowSize(m_window, &width, &height);
        return width;
    }

    unsigned int GLFWWindowWrapper::getHeight() const
    {
        int width, height;
        glfwGetWindowSize(m_window, &width, &height);
        return height;
    }

    void GLFWWindowWrapper::setVSync(bool enabled)
    {
        m_isVSync = enabled;
        glfwSwapInterval(m_isVSync ? 1 : 0);
    }

    bool GLFWWindowWrapper::isVSync() const
    {
        return m_isVSync;
    }

    bool GLFWWindowWrapper::isCloseRequested() const
    {
        return m_isCloseRequested;
    }

    void GLFWWindowWrapper::closeRequested()
    {
        m_isCloseRequested = true;
    }

    void GLFWWindowWrapper::close()
    {
        glfwSetWindowShouldClose(m_window, true);
    }

    bool GLFWWindowWrapper::isClosed() const
    {
        return glfwWindowShouldClose(m_window);
    }

} // namespace comet
