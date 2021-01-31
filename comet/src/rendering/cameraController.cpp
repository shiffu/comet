#define GLM_FORCE_SWIZZLE

#include <comet/cameraController.h>
#include <comet/log.h>

namespace comet
{
        #define BIND_METHOD(m) std::bind(&m, this, std::placeholders::_1)

        const glm::mat4& CameraController::getView() const
        {
            return m_view;
        }

        void CameraController::calculateView()
        {
            auto up = glm::vec3(0.0f, 1.0f, 0.0f);
            auto front = glm::normalize(m_focusTarget - m_position);
            auto right = glm::normalize(glm::cross(front, up));

            // If the angle between right vector and previous one (previous frame)
            // is greater than 90 degrees, then it means that the angle between up and
            // front vector is greater than 180 degrees and we need to invert the right vector
            if (m_right != glm::vec3(0.0f) && glm::dot(m_right, right) < 0)
            {
                right *= -1.0f;
            }
            m_up = glm::cross(right, front);
            m_right = right;

            m_view[0] = glm::vec4(right.x, m_up.x, -front.x, 0.0f);
            m_view[1] = glm::vec4(right.y, m_up.y, -front.y, 0.0f);
            m_view[2] = glm::vec4(right.z, m_up.z, -front.z, 0.0f);

            m_view[3][0] = -glm::dot(right, m_position);
            m_view[3][1] = -glm::dot(m_up, m_position);
            m_view[3][2] =  glm::dot(front, m_position);
        }

        const glm::mat4& CameraController::getProjection() const
        {
            return m_camera.getProjection();
        }

        const glm::vec3& CameraController::getPosition() const
        {
            return m_position;
        }

        void CameraController::setPosition(const glm::vec3& pos)
        {
            m_lastSetPosition = pos;
            m_position = pos;
            moveAroundTarget(0.0f, 0.0f);
            calculateView();
        }

        void CameraController::reset()
        {
            m_camera.setZoom(1.0f);
            setPosition(m_lastSetPosition);
            calculateView();
        }

        void CameraController::move(const glm::vec4& localTranslation)
        {
            auto cameraTransform = glm::inverse(m_view);
            glm::vec4 worldTranslation = cameraTransform * localTranslation;
            m_focusTarget += worldTranslation.xyz();
            m_position += worldTranslation.xyz();
            calculateView();
        }

        void CameraController::moveAroundTarget(float yaw, float pitch)
        {
            m_yaw += yaw;
            m_pitch += pitch;

            glm::vec3 fromTarget = m_position - m_focusTarget;
            float hDistanceFromTarget = glm::length(fromTarget) * cos(m_pitch);
            float vDistanceFromTarget = glm::length(fromTarget) * sin(m_pitch);
            float xOffset = hDistanceFromTarget * sin(m_yaw);
            float zOffset = hDistanceFromTarget * cos(m_yaw);

            m_position.x = m_focusTarget.x - xOffset;
            m_position.y = m_focusTarget.y + vDistanceFromTarget;
            m_position.z = m_focusTarget.z - zOffset;
            calculateView();
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

            return true;
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
                    return true;
                }

                auto deltaX = m_previousMouseX - x;
                auto deltaY = m_previousMouseY - y;

                if (m_rotate)
                {
                    moveAroundTarget(m_rotationSpeed * deltaX, -m_rotationSpeed * deltaY);
                }
                
                if (m_pan)
                {
                    auto correctedMoveSpeed = m_moveSpeed * m_camera.getZoom();
                    // CM_LOG_DEBUG("zoom: {}, moveSpeed: {}", m_camera.getZoom(), moveSpeed);
                    move({correctedMoveSpeed * deltaX, -correctedMoveSpeed * deltaY, 0.0f, 0.0f});
                }

                m_previousMouseX = (float)x;
                m_previousMouseY = (float)y;
            }

            return true;
        }

        bool CameraController::onVerticalMouseWheelScrolled(VerticalMouseWheelScrolledEvent& e)
        {
            m_camera.increaseZoom(-e.getDelta() * m_zoomSpeed);

            return true;
        }

        bool CameraController::onMouseButtonPressed(MouseButtonPressedEvent& e)
        {
            if (e.getButton() == Input::MouseButton::Middle && Input::isKeyPressed(Input::Key::LShift))
            {
                m_pan = true;
                m_rotate = false;
            }
            else if (e.getButton() == Input::MouseButton::Middle)
            {
                m_rotate = true;
            }

            return true;
        }

        bool CameraController::onMouseButtonRelease(MouseButtonReleasedEvent& e)
        {
            if (e.getButton() == Input::MouseButton::Middle)
            {
                m_rotate = false;
                m_pan = false;
                m_previousMouseX = 0.0f;
            }

            return true;
        }

} // namespace comet
