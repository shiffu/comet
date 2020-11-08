#include <comet/camera.h>
#include <comet/application.h>
#include <comet/log.h>

namespace comet
{

    // Perpective matrix
    Camera::Camera(float fov, float near, float far)
        : m_fov(fov), m_near(near), m_far(far)
    {
        auto app = Application::getInstance();
        m_width = app->getWindow().getWidth();
        m_height = app->getWindow().getHeight();
        m_projection = glm::perspective(m_fov, m_width / m_height, m_near, m_far);
    }

    // Orthographic matrix
    Camera::Camera(float near, float far)
        : m_near(near), m_far(far), m_fov(0.0f)
    {
        auto app = Application::getInstance();
        m_width = app->getWindow().getWidth();
        m_height = app->getWindow().getHeight();
        m_projection = glm::ortho(0.0f, m_width, 0.0f, m_height, m_near, m_far);
    }

    void Camera::setPerspective(float fov, float width, float height, float near, float far)
    {
        m_fov = fov;
        m_width = width;
        m_height = height;
        m_near = near;
        m_far = far;
        m_projection = glm::perspective(m_fov, m_width / m_height, m_near, m_far);
    }

    void Camera::setPerspective(float fov, float near, float far)
    {
        auto app = Application::getInstance();
        m_width = app->getWindow().getWidth();
        m_height = app->getWindow().getHeight();
        m_fov = fov;
        m_near = near;
        m_far = far;
        m_projection = glm::perspective(m_fov, m_width / m_height, m_near, m_far);
    }

    void Camera::setOrthographic(float left, float right, float bottom, float top, float near, float far)
    {
        m_fov = 0.0f;
        m_width = right - left;
        m_height = top - bottom;
        m_near = near;
        m_far = far;
        m_projection = glm::ortho(0.0f, m_width, 0.0f, m_height, m_near, m_far);
    }

    void Camera::setOrthographic(float near, float far)
    {
        auto app = Application::getInstance();
        m_width = app->getWindow().getWidth();
        m_height = app->getWindow().getHeight();
        m_fov = 0.0f;
        m_near = near;
        m_far = far;
        m_projection = glm::ortho(0.0f, m_width, 0.0f, m_height, m_near, m_far);
    }

    void Camera::lookAt(const glm::vec3& cameraPos, const glm::vec3& targetPos, const glm::vec3& up)
    {
        m_position = cameraPos;
        m_target = targetPos;
        m_up = up;
        calculateDirections();
        calculatedAngles();
        calculateView();
    }

    void Camera::lookAt(const glm::vec3& targetPos, const glm::vec3& up)
    {
        m_target = targetPos;
        m_up = up;
        calculateDirections();
        calculatedAngles();
        calculateView();
    }

    void Camera::lookAt(const glm::vec3& targetPos)
    {
        m_target = targetPos;
        calculateDirections();
        calculatedAngles();
        calculateView();
    }

    void Camera::translate(const glm::vec3& translation)
    {
        m_position -= translation;
        calculateView();
    }

    void Camera::setPosition(const glm::vec3& pos)
    {
        m_position = pos;
        calculateDirections();
        calculateView();
    }

    void Camera::setYaw(float yaw)
    {
        m_yaw = yaw;
        m_front.x = cos(m_yaw) * cos(m_pitch);
        m_front.y = sin(m_pitch);
        m_front.z = sin(m_yaw) * cos(m_pitch);

        m_front = glm::normalize(m_front);
        m_right = glm::normalize(glm::cross(m_up, m_front));
        m_up = glm::cross(m_front, m_right);
        calculateView();
    }
    
    void Camera::setPitch(float pitch)
    {
        m_pitch = pitch;
        m_front.x = cos(m_yaw) * cos(m_pitch);
        m_front.y = sin(m_pitch);
        m_front.z = sin(m_yaw) * cos(m_pitch);
        m_right = glm::normalize(glm::cross(m_up, m_front));
        m_up = glm::cross(m_front, m_right);
        calculateView();
    }

    void Camera::calculateDirections()
    {
        m_front = glm::normalize(m_position - m_target);
        m_right = glm::normalize(glm::cross(m_up, m_front));
        m_up = glm::cross(m_front, m_right);
    }

    void Camera::calculatedAngles()
    {
        m_pitch = asin(m_front.y);
        m_yaw = -acos(m_front.x / cos(m_pitch));
    }

    void Camera::calculateView()
    {
        glm::mat4 pos(1.0f);
        glm::mat4 rotation(1.0f);
        pos[3] = glm::vec4(-m_position, 1.0f);
        rotation[0] = glm::vec4(m_right.x, m_up.x, m_front.x, 0.0f);
        rotation[1] = glm::vec4(m_right.y, m_up.y, m_front.y, 0.0f);
        rotation[2] = glm::vec4(m_right.z, m_up.z, m_front.z, 0.0f);
        rotation[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
        m_view = rotation * pos;
    }

} // namespace comet
