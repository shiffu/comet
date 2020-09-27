#include <glad/glad.h>
#include "sandboxApp.h"
#include <string>

void SanboxApp::onStart()
{
    std::string vertexShaderSrc = R"glsl(
        #version 430 core
        
        layout (location = 0) in vec2 position;
        layout (location = 1) in vec3 color;

        out vec3 fragColor;

        void main() {
            fragColor = color;
            gl_Position = vec4(position, 0.0, 1.0);
        }
    )glsl";

    std::string fragmentShaderSrc = R"glsl(
        #version 430 core
        
        in vec3 fragColor;
        out vec4 color;

        void main() {
            color = vec4(fragColor, 1.0);
        }
    )glsl";

    m_shader = std::make_unique<comet::Shader>();
    m_shader->compileShaderSrc(vertexShaderSrc, GL_VERTEX_SHADER);
    m_shader->compileShaderSrc(fragmentShaderSrc, GL_FRAGMENT_SHADER);
    m_shader->linkProgram();


    float data[] = { -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
                      0.5f, -0.5f, 0.0f, 1.0f, 0.7f,
                      0.0f,  0.5f, 1.0f, 0.0f, 0.0f};

    m_vb = std::make_unique<comet::VertexBuffer>((const void*)data, sizeof(data));
    // m_vb->loadData();

    m_vao = std::make_unique<comet::VertexArray>();
    comet::VertexBufferLayout vbl;
    vbl.add<float>(2, GL_FALSE);
    vbl.add<float>(3, GL_FALSE);
    m_vao->addLayout(*m_vb, vbl);
}

void SanboxApp::onUpdate()
{
    m_shader->bind();
    m_vao->bind();
    glDrawArrays(GL_TRIANGLES, 0, 3);
    // m_shader->unbind();
}