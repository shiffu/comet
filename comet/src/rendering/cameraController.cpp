#include <comet/cameraController.h>
#include <comet/log.h>
#include <glm/matrix.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_access.hpp>
#include <glm/gtc/matrix_transform.hpp>


namespace comet
{
    
        #define BIND_METHOD(m) std::bind(&m, this, std::placeholders::_1)

        glm::mat4 CameraController::getView() const
        {
            return glm::inverse(m_transform);
        }

        const glm::mat4& CameraController::getProjection() const
        {
            return m_camera.getProjection();
        }

        const glm::vec3& CameraController::getPosition() const
        {
            return (glm::vec3&)(m_transform[3]);
        }

        void CameraController::setPosition(const glm::vec3& pos)
        {
            m_lastSetPosition = pos;
            m_transform[3] = glm::vec4(pos.x, pos.y, pos.z, 1.0f);
        }

        void CameraController::reset()
        {
            m_camera.setZoom(1.0f);
            m_transform = glm::translate(glm::mat4(1.0f), m_lastSetPosition);
        }

        void CameraController::move(const glm::vec3& translation)
        {
            m_transform = glm::translate(m_transform, translation);
        }

        void CameraController::addYaw(float yaw)
        {
            m_transform = glm::rotate(m_transform, yaw, glm::vec3(0.0f, 1.0f, 0.0f));
        }

        void CameraController::addPitch(float pitch)
        {
            m_transform = glm::rotate(m_transform, pitch, glm::vec3(1.0f, 0.0f, 0.0f));
        }

        void CameraController::addRoll(float roll)
        {
            m_transform = glm::rotate(m_transform, roll, glm::vec3(0.0f, 0.0f, 1.0f));
        }

        void CameraController::onUpdate(double deltaTime)
        {
        }

        void CameraController::onEvent(Event& e)
        {
            EventDispatcher dispatcher(e);

            dispatcher.dispatch<WindowResizedEvent>(BIND_METHOD(CameraController::onWindowResized));
            dispatcher.dispatch<KeyPressedEvent>(BIND_METHOD(CameraController::onKeyPressed));
            dispatcher.dispatch<MouseMovedEvent>(BIND_METHOD(CameraController::onMouseMoved));
            dispatcher.dispatch<VerticalMouseWheelScrolledEvent>(BIND_METHOD(CameraController::onVerticalMouseWheelScrolled));
            dispatcher.dispatch<MouseButtonPressedEvent>(BIND_METHOD(CameraController::onMouseButtonPressed));
            dispatcher.dispatch<MouseButtonReleasedEvent>(BIND_METHOD(CameraController::onMouseButtonRelease));
        }

        bool CameraController::onWindowResized(WindowResizedEvent& e)
        {
            m_camera.setAspectRatio((float)e.getWidth() / (float)e.getHeight());
        }

        bool CameraController::onKeyPressed(KeyPressedEvent& e)
        {
            if (e.getKeyCode() == Input::Key::R)
            {
                reset();
            }
        }

        bool CameraController::onMouseMoved(MouseMovedEvent& e)
        {
            if (m_rotate || m_pan)
            {
                auto [x, y] = e.getPosition();
                if (m_previousMouseX == 0.0f)
                {
                    m_previousMouseX = (float)x;
                    m_previousMouseY = (float)y;
                }

                auto deltaX = m_previousMouseX - x;
                auto deltaY = m_previousMouseY - y;

                if (m_rotate)
                {
                    addYaw(glm::radians(10.0f) * m_rotationSpeed * deltaX);
                    addPitch(glm::radians(10.0f) * m_rotationSpeed * deltaY);
                }
                else
                {
                    move(glm::vec3(m_moveSpeed * deltaX, -m_moveSpeed * deltaY, 0.0f));
                }

                m_previousMouseX = (float)x;
                m_previousMouseY = (float)y;
            }
        }

        bool CameraController::onVerticalMouseWheelScrolled(VerticalMouseWheelScrolledEvent& e)
        {
            m_camera.increaseZoom(-e.getDelta() * m_zoomSpeed);
        }

        bool CameraController::onMouseButtonPressed(MouseButtonPressedEvent& e)
        {
            if (e.getButton() == Input::MouseButton::Middle && Input::isKeyPressed(Input::Key::LShift))
            {
                m_pan = true;
            }
            else if (e.getButton() == Input::MouseButton::Middle)
            {
                m_rotate = true;
            }
        }

        bool CameraController::onMouseButtonRelease(MouseButtonReleasedEvent& e)
        {
            if (e.getButton() == Input::MouseButton::Middle)
            {
                m_rotate = false;
                m_pan = false;
                m_previousMouseX = 0.0f;
            }
        }

} // namespace comet
