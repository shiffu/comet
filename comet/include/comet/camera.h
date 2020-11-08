#pragma once

#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace comet
{

    class Camera
    {
    public:
        Camera() {}
        Camera(float near, float far);
        Camera(float fov, float near, float far);

        const glm::mat4& getProjection() const { return m_projection; }
        const glm::mat4 getViewProjection() const { return m_projection * m_view; }
        void setProjection(const glm::mat4& projection) { m_projection = m_projection; }
        void setPerspective(float fov, float width, float height, float near, float far);
        void setPerspective(float fov, float near, float far);
        void setOrthographic(float left, float right, float bottom, float top, float near, float far);
        void setOrthographic(float near, float far);

        const glm::vec3& getTarget() { return m_target; };
        void lookAt(const glm::vec3& cameraPos, const glm::vec3& target, const glm::vec3& up);
        void lookAt(const glm::vec3& target, const glm::vec3& up);
        void lookAt(const glm::vec3& target);

        glm::mat4& getView() { return m_view; }
        void setView(const glm::mat4& view) { m_view = view; }

        void translate(const glm::vec3& translation);
        const glm::vec3& getPosition() const { return m_position; }
        void setPosition(const glm::vec3& pos);

        float getYaw() const { return m_yaw; }
        void setYaw(float yaw);
        float getPitch() const { return m_pitch; }
        void setPitch(float pitch);

        const glm::vec3& getUp() const { return m_up; }
        void setUp(const glm::vec3& up) { m_up = up; }

        const glm::vec3& getFront() const { return m_front; }
        void setFront(const glm::vec3& front) { m_front = front; calculateView(); }

        const glm::vec3& getRight() const { return m_right; }
        void setRight(const glm::vec3& right) { m_right = right; calculateView(); }
        
        float getRatio() const { return m_width / m_height; }

        float getWidth() const { return m_width; }
        void setWidth(float width) { m_width = width; calculateView(); }

        float getHeight() const { return m_height; }
        void setHeight(float height) { m_height = height; calculateView(); }

        float getFOV() const { return m_fov; }
        void setFOV(float fov) { m_fov = fov; calculateView(); }

        float getNear() const { return m_near; }
        void setNear(float near) { m_near = near; calculateView(); }

        float getFar() const { return m_far; }
        void setFar(float far) { m_far = far; calculateView(); }

    private:
        void calculateView();
        void calculatedAngles();
        void calculateDirections();

    private:
        glm::mat4 m_view{1.0f};
        glm::mat4 m_projection{1.0f};
        float m_fov{0.0f};
        float m_width{0.0f};
        float m_height{0.0f};
        float m_near{0.1f};
        float m_far{500.0f};
        float m_pitch{0.0f};
        float m_yaw{0.0f};
        glm::vec3 m_position{0.0f, 0.0f, -0.5f};
        glm::vec3 m_target{0.0f};
        glm::vec3 m_up{0.0f, 1.0f, 0.0f};
        glm::vec3 m_front{0.0f, 0.0f, 1.0f};
        glm::vec3 m_right{1.0f, 0.0f, 0.0f};
    };
    
} // namespace comet
