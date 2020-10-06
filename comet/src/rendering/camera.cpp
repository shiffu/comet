#include <comet/camera.h>


namespace comet
{

    Camera::Camera(float fov, float width, float height, float near, float far)
        : m_fov(fov), m_width(width), m_height(height), m_near(near), m_far(far)
    {
        m_projection = glm::perspective(m_fov, m_width / m_height, m_near, m_far);
    }

    void Camera::lookAt(const glm::vec3& cameraPos, const glm::vec3& lookAtPos, const glm::vec3& up)
    {
        m_position = cameraPos;
        m_lookAt = lookAtPos;
        m_up = up;
        m_view = glm::lookAt(m_position, m_lookAt, m_up);
    }

    void Camera::lookAt(const glm::vec3& lookAtPos, const glm::vec3& up)
    {
        m_lookAt = lookAtPos;
        m_up = up;
        m_view = glm::lookAt(m_position, m_lookAt, m_up);
    }

    void Camera::recalculateView()
    {
        m_view = glm::lookAt(m_position, m_lookAt, m_up);
    }

} // namespace comet
