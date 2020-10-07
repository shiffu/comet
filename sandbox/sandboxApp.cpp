#include "sandboxApp.h"
#include <string>
#include <math.h>
#include <comet/input.h>
#include "vertex.h"

comet::Application* comet::Application::getInstance()
{
    static SandboxApp* instance = new SandboxApp();
    return instance;
}

void SandboxApp::onStart()
{
    std::string vertexShaderSrc = R"(
        #version 430 core
        
        layout (location = 0) in vec3 position;
        layout (location = 1) in vec3 color;

        uniform vec3 offset;
        uniform mat4 mvp;
        out vec3 fragColor;

        void main()
        {
            fragColor = color;
            gl_Position = mvp * vec4(position + offset, 1.0);
        }
    )";

    std::string fragmentShaderSrc = R"(
        #version 430 core
        
        in vec3 fragColor;
        out vec4 color;

        void main()
        {
            color = vec4(fragColor, 1.0);
        }
    )";

    m_shader = std::make_unique<comet::Shader>();
    m_shader->compileShaderSrc(vertexShaderSrc, GL_VERTEX_SHADER);
    m_shader->compileShaderSrc(fragmentShaderSrc, GL_FRAGMENT_SHADER);
    m_shader->linkProgram();


    Vertex data[] = {
        {{-0.5f, -0.5f, 0.0f},  {0.0f, 0.2f, 0.7f}},
        {{0.5f, -0.5f, 0.0f},   {0.0f, 0.2f, 0.7f}},
        {{0.5f,  0.5f, 0.0f},   {0.8f, 0.4f, 0.0f}},
        {{-0.5f,  0.5f, 0.0f},  {0.8f, 0.4f, 0.0f}}
    };

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
    m_vao->addLayout(vbl, *m_vb, *m_ib);

    m_camera.lookAt(glm::vec3{0.0f, 0.0f, -7.0f}, glm::vec3{0.0f, 0.0f, 1.0f}, glm::vec3{0.0f, 1.0f, 0.0f});
}

bool SandboxApp::onKeyPressed(comet::KeyPressedEvent& e)
{
}

bool SandboxApp::onVerticalMouseWheelScrolled(comet::VerticalMouseWheelScrolledEvent& e)
{
    auto pos = m_camera.getPosition();
    pos.z += e.getDelta() * 0.8f;
    m_camera.setPosition(pos);
}

void SandboxApp::onUpdate(double deltaTime)
{
    if (comet::Input::isKeyPressed(comet::Input::Key::A))
    {
        m_offset.x += 0.005f * deltaTime;
    }
    else if (comet::Input::isKeyPressed(comet::Input::Key::D))
    {
        m_offset.x -= 0.005f * deltaTime;
    }

    if (comet::Input::isKeyPressed(comet::Input::Key::S))
    {
        m_offset.y -= 0.005f * deltaTime;
    }
    else if (comet::Input::isKeyPressed(comet::Input::Key::W))
    {
        m_offset.y += 0.005f * deltaTime;
    }
}

void SandboxApp::onFixedUpdate(float fixedDeltaTime)
{
}

void SandboxApp::onRender()
{
    m_shader->bind();

    glm::mat4 model{1.0f};
    glm::mat4 mvp = m_camera.getProjection() * m_camera.getView() * model;
    m_shader->setUniform("mvp", mvp);
    m_shader->setUniform("offset", m_offset);
    m_vao->bind();

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)nullptr);
}