#pragma once

#include <comet/comet.h>
#include <memory>
#include <glm/vec3.hpp>

class SandboxApp : public comet::Application
{
public:

    SandboxApp() { CM_LOG_DEBUG("SandboxApp contructor"); } ;
    virtual ~SandboxApp();

    virtual void onStart() override;
    virtual void onImGuiDraw() override;
    virtual void onUpdate(double deltaTime) override;

    virtual void onEvent(comet::Event& e) override;
    virtual bool onKeyPressed(comet::KeyPressedEvent& e) override;

private:
    double m_angle{0.0};
    glm::vec3 m_offset{0.0f};
    comet::Camera m_camera;
    comet::CameraController m_cameraController{m_camera};
    bool m_pauseAnimation{false};
    std::unique_ptr<comet::DirectionalLight> m_directionalLight{};
    std::vector<std::unique_ptr<comet::PointLight>> m_pointLights{};
    std::vector<std::unique_ptr<comet::SpotLight>> m_spotLights{};
};