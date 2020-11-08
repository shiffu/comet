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
    double m_angle{0.0};
    glm::vec3 m_offset{0.0f};
    comet::Camera m_camera;
    comet::Renderer m_renderer;
    comet::Mesh* m_quad;
    comet::Mesh* m_terrain;
};