#include <comet/camera.h>
#include <comet/application.h>
#include <comet/log.h>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/rotate_vector.hpp>

namespace comet
{

    // Perpective matrix
    Camera::Camera(float fov, float near, float far)
    {
        updateDirections();
        setPerspective(fov, near, far);
    }

    // Orthographic matrix
    Camera::Camera(float near, float far)
    {
        updateDirections();
        setOrthographic(near, far);
    }

    void Camera::updateWidthAndHeightFromWindow()
    {
        auto app = Application::getInstance();
        m_width = app->getWindow().getWidth();
        m_height = app->getWindow().getHeight();
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
        updateWidthAndHeightFromWindow();
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
        updateWidthAndHeightFromWindow();
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
        calculateDirectionsFromTarget();
        buildViewMatrix();
    }

    void Camera::lookAt(const glm::vec3& targetPos, const glm::vec3& up /*= glm::vec3{0.0f, 1.0f, 0.0f}*/)
    {
        m_target = targetPos;
        m_up = up;
        calculateDirectionsFromTarget();
        
        m_yaw = glm::angle(m_right, -glm::normalize(m_position - m_target));
        m_pitch = glm::angle(m_front, -glm::normalize(m_position - m_target));

        buildViewMatrix();
    }

    void Camera::move(const glm::vec3& translation)
    {
        setPosition(m_position + translation);
    }

    void Camera::setPosition(const glm::vec3& pos)
    {
        m_position = pos;
        buildViewMatrix();
    }

    void Camera::moveFront(float delta)
    {
        auto dir = glm::normalize(m_front);
        dir *= delta;
        move(dir);
    }

    void Camera::moveRight(float delta)
    {
        auto dir = glm::normalize(m_right);
        dir *= delta;
        move(dir);
    }

    void Camera::moveUp(float delta)
    {
        auto dir = glm::normalize(m_up);
        dir *= delta;
        move(dir);
    }

    void Camera::addYaw(float yaw)
    {
        m_yaw = yaw;
        m_front = glm::rotate(glm::normalize(m_front), m_yaw, m_up);
        updateDirections();
        buildViewMatrix();
    }
    
    void Camera::addPitch(float pitch)
    {
        m_pitch = pitch;
        m_front = glm::rotate(glm::normalize(m_front), m_pitch, m_right);
        updateDirections();
        buildViewMatrix();
    }
    
    void Camera::addRoll(float roll)
    {
        m_roll = roll;
        m_up = glm::rotate(glm::normalize(m_up), -m_roll, m_front);
        updateDirections();
        buildViewMatrix();
    }

    void Camera::setUp(const glm::vec3& up)
    {
        m_up = up;
        updateDirections();
        buildViewMatrix();
    }

    void Camera::setFront(const glm::vec3& front)
    {
        m_front = front;
        updateDirections();
        buildViewMatrix();
    }

    void Camera::setRight(const glm::vec3& right)
    {
        m_right = right;
        updateDirections();
        buildViewMatrix();
    }

    void Camera::updateDirections()
    {
        m_front = glm::normalize(m_front);
        m_right = glm::normalize(glm::cross(m_up, m_front));
        m_up = glm::cross(m_front, m_right);
    }

    void Camera::calculateDirectionsFromTarget()
    {
        m_front = glm::normalize(m_position - m_target);
        m_right = glm::normalize(glm::cross(m_up, m_front));
        m_up = glm::cross(m_front, m_right);
    }

    void Camera::buildViewMatrix()
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
