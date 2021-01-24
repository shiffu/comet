#pragma once

#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>

namespace comet
{

    class Camera
    {
    public:
        enum ProjectionType
        {
            PERSPECTIVE = 0,
            ORTHOGRAPHIC
        };

        Camera(ProjectionType type = ProjectionType::PERSPECTIVE)
            : m_projectionType(type) {};

        ProjectionType getProjectionType() const { return m_projectionType; }
        void setProjectionType(ProjectionType type) { m_projectionType = type; }
        const glm::mat4& getProjection() const { return m_projection; }
        void setProjection(const glm::mat4& projection) { m_projection = m_projection; }

        void setPerspective(float fov, float aspectRatio, float near, float far);
        void setPerspective(float fov, float width, float height, float near, float far);

        void setOrthographic(float left, float right, float bottom, float top, float near, float far);
        void setOrthographic(float width, float height, float near, float far);
        void setOrthographic(float aspectRatio, float near, float far);

        float getAspectRatio() const { return m_aspectRatio; }
        void setAspectRatio(float aspectRatio) { m_aspectRatio = aspectRatio; calculateProjection(); }

        float getFOV() const { return m_fov; }
        void setFOV(float fov) { m_fov = fov; calculatePerspectiveProjection(); }

        float getZoom() const { return m_zoom; }
        void setZoom(float zoom) { m_zoom = zoom; calculateProjection(); }
        void increaseZoom(float zoomDelta) { m_zoom += zoomDelta; m_zoom = std::max(0.01f, m_zoom); calculateProjection(); }

        float getNear() const { return m_near; }
        void setNear(float near) { m_near = near; calculateProjection(); }

        float getFar() const { return m_far; }
        void setFar(float far) { m_far = far; calculateProjection(); }

    private:
        void calculateProjection();    
        void calculatePerspectiveProjection();
        void calculateOrthographicProjection();

    private:
        glm::mat4 m_projection{1.0f};
        ProjectionType m_projectionType;
        float m_zoom{1.0f};
        float m_fov{glm::radians(60.0f)};
        float m_aspectRatio{1.0f};
        float m_near{0.1f};
        float m_far{500.0f};
    };
    
} // namespace comet
