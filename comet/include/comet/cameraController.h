#pragma once

#include <comet/camera.h>
#include <comet/event.h>
#include <glm/mat4x4.hpp>

namespace comet
{
    class CameraController
    {
    public:
        CameraController(Camera& camera) : m_camera(camera) {}
        ~CameraController() = default;

        const glm::mat4& getTransform() const { return m_transform; }
        void setTransform(const glm::mat4& transform) { m_transform = transform; }
        void reset();

        glm::mat4 getView() const;
        const glm::mat4& getProjection() const;

        void onUpdate(double deltaTime);

        void onEvent(Event& e);
        bool onWindowResized(WindowResizedEvent& e);
        bool onKeyPressed(KeyPressedEvent& e);
        bool onMouseMoved(MouseMovedEvent& e);
        bool onVerticalMouseWheelScrolled(VerticalMouseWheelScrolledEvent& e);
        bool onMouseButtonPressed(MouseButtonPressedEvent& e);
        bool onMouseButtonRelease(MouseButtonReleasedEvent& e);

        const glm::vec3& getPosition() const;
        void setPosition(const glm::vec3& pos);
        void move(const glm::vec3& translation);
        
        void setMoveSpeed(float moveSpeed) {m_moveSpeed = moveSpeed; }
        float getMoveSpeed() const noexcept { return m_moveSpeed; }
        
        void setRotationSpeed(float rotationSpeed) {m_rotationSpeed = rotationSpeed; }
        float getRotationSpeed() const noexcept { return m_rotationSpeed; }
        
        void setZoomSpeed(float zoomSpeed) {m_zoomSpeed = zoomSpeed; }
        float getZoomSpeed() const noexcept { return m_zoomSpeed; }

        void addYaw(float yaw);
        void addPitch(float pitch);
        void addRoll(float pitch);

    private:
        glm::mat4 m_transform{1.0f};
        glm::vec3 m_lastSetPosition;
        float m_moveSpeed{0.04f};
        float m_rotationSpeed{0.04f};
        float m_zoomSpeed{0.05f};
        float m_previousMouseX{0.0f};
        float m_previousMouseY{0.0f};
        bool m_pan{false};
        bool m_rotate{false};
        Camera& m_camera;
    };

} // namespace comet
