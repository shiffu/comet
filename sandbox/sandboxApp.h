#pragma once

#include <comet/comet.h>
#include <glm/vec3.hpp>

class SandboxApp : public comet::Application
{
public:

    SandboxApp() { CM_LOG_DEBUG("SandboxApp contructor"); } ;
    virtual ~SandboxApp();

    void onStart() override;
    void onImGuiDraw() override;
    void onUpdate(double deltaTime) override;
    void onFixedUpdate(float fixedDeltaTime) override;
    void onRender() override;

    virtual bool onKeyPressed(comet::KeyPressedEvent& e);
    virtual bool onVerticalMouseWheelScrolled(comet::VerticalMouseWheelScrolledEvent& e);

private:
    double m_angle{0.0};
    glm::vec3 m_offset{0.0f};
    comet::Camera m_camera;
    comet::Renderer m_renderer;
    comet::FlatColorMaterial m_blueColorMaterial{glm::vec4(0.0f, 0.5, 1.0f, 1.0f)};
    comet::FlatColorMaterial m_lightBlueColorMaterial{glm::vec4(0.0f, 0.8, 1.0f, 1.0f)};
    comet::FlatColorMaterial m_redColorMaterial{glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)};
    comet::PhongMaterial m_phongMaterial;
    comet::Mesh* m_quad;
    comet::Mesh* m_cube;
    comet::Mesh* m_test;
    comet::Mesh* m_terrain;
    bool m_pauseAnimation{false};
};