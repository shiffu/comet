#pragma once

#include <comet/comet.h>
#include <memory>

class SanboxApp : public comet::Application
{
public:
    SanboxApp() { CM_LOG_DEBUG("SanboxApp contructor"); } ;
    virtual ~SanboxApp() { CM_LOG_DEBUG("SanboxApp destructor"); };

    void onStart() override;
    void onUpdate(double deltaTime) override;
    void onFixedUpdate(float fixedDeltaTime) override;
    void onRender() override;

private:
    std::unique_ptr<comet::Shader> m_shader;
    std::unique_ptr<comet::VertexBuffer> m_vb;
    std::unique_ptr<comet::IndexBuffer> m_ib;
    std::unique_ptr<comet::VertexArray> m_vao;
    double m_angle{0.0};
};