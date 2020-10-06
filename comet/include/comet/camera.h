#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>

namespace comet
{

    class Camera
    {
    public:
        Camera(float fov, float width, float height, float near, float far);

        const glm::vec3& getLookAt() const { return m_lookAt; }
        void lookAt(const glm::vec3& cameraPos, const glm::vec3& lookAtPos, const glm::vec3& up);
        void lookAt(const glm::vec3& lookAtPos, const glm::vec3& up);

        const glm::mat4& getProjection() const { return m_projection; }
        void setProjection(const glm::mat4& projection) { m_projection = m_projection; }

        const glm::mat4& getView() const { return m_view; }
        void setView(const glm::mat4& view) { m_view = view; }

        const glm::vec3& getPosition() const { return m_position; }
        void setPosition(const glm::vec3& pos) {m_position = pos; recalculateView(); }

        const glm::vec3& getUp() const { return m_up; }
        
        float getRatio() const { return m_width / m_height; }

        float getWidth() const { return m_width; }
        void setWidth(float width) { m_width = width; }

        float getHeight() const { return m_height; }
        void setHeight(float height) { m_height = height; }

        float getFOV() const { return m_fov; }
        void setFOV(float fov) { m_fov = fov; }

        float getNear() const { return m_near; }
        void setNear(float near) { m_near = near; }

        float getFar() const { return m_far; }
        void setFar(float far) { m_far = far; }

    private:
        void recalculateView();

    private:
        glm::mat4 m_view{1};
        glm::mat4 m_projection{1};
        float m_fov;
        float m_width;
        float m_height;
        float m_near;
        float m_far;
        glm::vec3 m_position;
        glm::vec3 m_lookAt;
        glm::vec3 m_up;
    };
    
} // namespace comet
