#include "sandboxApp.h"
#include <string>
#include <math.h>
#include <comet/input.h>

comet::Application* comet::Application::getInstance()
{
    static SandboxApp* instance = new SandboxApp();
    return instance;
}

void SandboxApp::onStart()
{
    std::string vertexShaderSrc = R"glsl(
        #version 430 core
        
        layout (location = 0) in vec3 position;
        layout (location = 1) in vec3 color;

        uniform vec3 offset;
        out vec3 fragColor;

        void main()
        {
            fragColor = color;
            gl_Position = vec4(position + offset, 1.0);
        }
    )glsl";

    std::string fragmentShaderSrc = R"glsl(
        #version 430 core
        
        in vec3 fragColor;
        out vec4 color;

        void main()
        {
            color = vec4(fragColor, 1.0);
        }
    )glsl";

    m_shader = std::make_unique<comet::Shader>();
    m_shader->compileShaderSrc(vertexShaderSrc, GL_VERTEX_SHADER);
    m_shader->compileShaderSrc(fragmentShaderSrc, GL_FRAGMENT_SHADER);
    m_shader->linkProgram();


    //              Positions           Colors
    float data[] = { -0.5f, -0.5f, 0.0f,      0.0f, 0.2f, 0.7f,
                      0.5f, -0.5f, 0.0f,      0.0f, 0.2f, 0.7f,
                      0.5f,  0.5f, 0.0f,      0.8f, 0.4f, 0.0f,
                     -0.5f,  0.5f, 0.0f,      0.8f, 0.4f, 0.0f};

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    m_vb = std::make_unique<comet::VertexBuffer>((const void*)data, sizeof(data));
    m_ib = std::make_unique<comet::IndexBuffer>((const unsigned int*)indices, sizeof(indices));

    m_vao = std::make_unique<comet::VertexArray>();
    comet::VertexBufferLayout vbl;
    vbl.add<float>(3, GL_FALSE);
    vbl.add<float>(3, GL_FALSE);
    m_vao->addLayout(*m_vb, vbl);
}

bool SandboxApp::onKeyPressed(comet::KeyPressedEvent& e)
{
    CM_LOG_TRACE(e.toString());
}

static const double PI = 3.1415926535;

void SandboxApp::onUpdate(double deltaTime)
{
    // m_angle += 2 * PI * (deltaTime / 1000.0);
    // if (m_angle > 2 * PI)
    // {
    //     m_angle = 0.0;
    // }
    if (comet::Input::isKeyPressed(comet::Input::Key::A))
    {
        m_offset.x -= 0.003f * deltaTime;
    }
    else if (comet::Input::isKeyPressed(comet::Input::Key::D))
    {
        m_offset.x += 0.003f * deltaTime;
    }

    if (comet::Input::isKeyPressed(comet::Input::Key::S))
    {
        m_offset.y -= 0.003f * deltaTime;
    }
    else if (comet::Input::isKeyPressed(comet::Input::Key::W))
    {
        m_offset.y += 0.003f * deltaTime;
    }

}

void SandboxApp::onFixedUpdate(float fixedDeltaTime)
{
    // m_angle += 2 * PI * (fixedDeltaTime / 1000.0);
    // if (m_angle > 2 * PI)
    // {
    //     m_angle = 0.0;
    // }
}

void SandboxApp::onRender()
{
    m_shader->bind();

    // float xOffset = cos(m_angle) * 0.4f;
    // glm::vec3 offset(xOffset, 0, 0);
    m_shader->setUniform("offset", m_offset);
    m_vao->bind();
    m_ib->bind();

    // glDrawArrays(GL_TRIANGLES, 0, 3);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)nullptr);
}