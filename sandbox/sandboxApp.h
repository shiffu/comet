#pragma once

#include <comet/comet.h>
#include <memory>
#include <glm/vec3.hpp>

class SandboxApp : public comet::Application
{
public:

    SandboxApp() { CM_LOG_DEBUG("SandboxApp contructor"); } ;
    virtual ~SandboxApp() { CM_LOG_DEBUG("SandboxApp destructor"); };

    void onStart() override;
    void onUpdate(double deltaTime) override;
    void onFixedUpdate(float fixedDeltaTime) override;
    void onRender() override;

    virtual bool onKeyPressed(comet::KeyPressedEvent& e);
    virtual bool onVerticalMouseWheelScrolled(comet::VerticalMouseWheelScrolledEvent& e);

private:
    std::unique_ptr<comet::Shader> m_shader;
    std::unique_ptr<comet::VertexBuffer> m_vb;
    std::unique_ptr<comet::IndexBuffer> m_ib;
    std::unique_ptr<comet::VertexArray> m_vao;
    double m_angle{0.0};
    glm::vec3 m_offset{0.0f};
    comet::Camera m_camera{glm::radians(45.0f), 1280.0f, 720.0f, 0.1f, 100.0f};
    comet::Renderer m_renderer;
};