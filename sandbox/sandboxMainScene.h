#pragma once

#include <comet/comet.h>

class SandboxMainScene : public comet::Scene
{
public:
    SandboxMainScene(const char* name) : comet::Scene(name) {}
    ~SandboxMainScene() = default;

    virtual bool onEvent(comet::Event& e) override;
    virtual bool onKeyPressed(comet::KeyPressedEvent& e) override;

    virtual void onStart() override;
    virtual void onRender() override;

    private:
        comet::Camera m_camera;
        comet::CameraController m_cameraController{m_camera};
        double m_angle{0.0};
        glm::vec3 m_offset{0.0f};
        bool m_pauseAnimation{false};
        std::unique_ptr<comet::DirectionalLight> m_directionalLight{};
        std::vector<std::unique_ptr<comet::PointLight>> m_pointLights{};
        std::vector<std::unique_ptr<comet::SpotLight>> m_spotLights{};
};