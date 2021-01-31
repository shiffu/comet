#pragma once

#include <comet/camera.h>
#include <comet/event.h>

#include <glm/glm.hpp>

namespace comet
{
    class CameraController
    {
    public:
        CameraController(Camera& camera) : m_camera(camera) {}
        ~CameraController() = default;

        void reset();

        const glm::mat4& getView() const;
        const glm::mat4& getProjection() const;

        void onEvent(Event& e);
        bool onWindowResized(WindowResizedEvent& e);
        bool onKeyPressed(KeyPressedEvent& e);
        bool onMouseMoved(MouseMovedEvent& e);
        bool onVerticalMouseWheelScrolled(VerticalMouseWheelScrolledEvent& e);
        bool onMouseButtonPressed(MouseButtonPressedEvent& e);
        bool onMouseButtonRelease(MouseButtonReleasedEvent& e);

        const glm::vec3& getTargetPosition() const { return m_focusTarget; }
        void setTargetPosition(const glm::vec3& pos) { m_focusTarget = pos; calculateView(); }

        const glm::vec3& getPosition() const;
        void setPosition(const glm::vec3& pos);
        void move(const glm::vec4& translation);
        
        void setMoveSpeed(float moveSpeed) {m_moveSpeed = moveSpeed; }
        float getMoveSpeed() const noexcept { return m_moveSpeed; }
        
        void setRotationSpeed(float rotationSpeed) {m_rotationSpeed = rotationSpeed; }
        float getRotationSpeed() const noexcept { return m_rotationSpeed; }
        
        void setZoomSpeed(float zoomSpeed) {m_zoomSpeed = zoomSpeed; }
        float getZoomSpeed() const noexcept { return m_zoomSpeed; }

    private:
        void calculateView();
        void moveAroundTarget(float yaw, float pitch);

    private:
        glm::vec3 m_lastSetPosition{0.0f};
        float m_moveSpeed{0.01f};
        float m_rotationSpeed{0.005f};
        float m_zoomSpeed{0.02f};
        float m_previousMouseX{0.0f};
        float m_previousMouseY{0.0f};
        bool m_pan{false};
        bool m_rotate{false};
        Camera& m_camera;

        float m_yaw{0};
        float m_pitch{0};
        glm::mat4 m_view{1.0f};
        glm::vec3 m_up{0.0f, 1.0f, 0.0f};
        glm::vec3 m_right{0.0f, 0.0f, 0.0f};
        glm::vec3 m_position{0.0f};
        glm::vec3 m_focusTarget{0.0f};
    };

} // namespace comet
