#include <comet/camera.h>
#include <glm/gtc/matrix_transform.hpp>

namespace comet
{

    void Camera::setPerspective(float fov, float width, float height, float near, float far)
    {
        m_fov = fov;
        m_aspectRatio = width / height;
        m_near = near;
        m_far = far;
        calculatePerspectiveProjection();
    }

    void Camera::setPerspective(float fov, float aspectRatio, float near, float far)
    {
        m_fov = fov;
        m_aspectRatio = aspectRatio;
        m_near = near;
        m_far = far;
        calculatePerspectiveProjection();
    }

    void Camera::setOrthographic(float left, float right, float bottom, float top, float near, float far)
    {
        m_fov = 0.0f;
        m_aspectRatio = (right - left) / (top - bottom);
        m_near = near;
        m_far = far;
        calculateOrthographicProjection();
    }

    void Camera::setOrthographic(float width, float height, float near, float far)
    {
        m_fov = 0.0f;
        m_aspectRatio = width / height;
        m_near = near;
        m_far = far;
        calculateOrthographicProjection();
    }

    void Camera::calculateProjection()
    {
        if (m_fov != 0.0f)
        {
            calculatePerspectiveProjection();
        }
        else
        {
            calculateOrthographicProjection();
        }
    }

    void Camera::calculatePerspectiveProjection()
    {
        m_projection = glm::perspective(m_fov * m_zoom, m_aspectRatio, m_near, m_far);
    }

    void Camera::calculateOrthographicProjection()
    {
        m_projection = glm::ortho(-m_aspectRatio * m_zoom, m_aspectRatio * m_zoom, -m_zoom, m_zoom, m_near, m_far);
    }
    
} // namespace comet
