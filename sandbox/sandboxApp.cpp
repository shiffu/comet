#include <glad/glad.h>
#include "sandboxApp.h"
#include <string>
#include <math.h>

void SanboxApp::onStart()
{
    std::string vertexShaderSrc = R"glsl(
        #version 430 core
        
        layout (location = 0) in vec2 position;
        layout (location = 1) in vec3 color;

        uniform float offset;
        out vec3 fragColor;

        void main()
        {
            fragColor = color;
            gl_Position = vec4(position.x + offset, position.y, 0.0, 1.0);
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


    float data[] = { -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
                      0.5f, -0.5f, 0.0f, 1.0f, 0.7f,
                      0.0f,  0.5f, 0.8f, 0.2f, 0.6f};

    m_vb = std::make_unique<comet::VertexBuffer>((const void*)data, sizeof(data));
    m_vao = std::make_unique<comet::VertexArray>();
    comet::VertexBufferLayout vbl;
    vbl.add<float>(2, GL_FALSE);
    vbl.add<float>(3, GL_FALSE);
    m_vao->addLayout(*m_vb, vbl);
}

static const double PI = 3.1415926535;

void SanboxApp::onUpdate(double deltaTime)
{
    m_angle += 2 * PI * (deltaTime / 1000.0);
    if (m_angle > 2 * PI)
    {
        m_angle = 0.0;
    }
}

void SanboxApp::onFixedUpdate(float fixedDeltaTime)
{
    // m_angle += 2 * PI * (fixedDeltaTime / 1000.0);
    // if (m_angle > 2 * PI)
    // {
    //     m_angle = 0.0;
    // }
}

void SanboxApp::onRender()
{
    m_shader->bind();

    float offset = cos(m_angle) * 0.4f;
    m_shader->setUniform("offset", offset);
    m_vao->bind();

    glDrawArrays(GL_TRIANGLES, 0, 3);
}