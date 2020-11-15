#include <glad/glad.h>

#include "sandboxApp.h"
#include <string>
#include <math.h>
#include <utility>
#include <comet/input.h>
#include <comet/vertex.h>

comet::Application* comet::Application::getInstance()
{
    static SandboxApp* instance = new SandboxApp();
    return instance;
}

SandboxApp::~SandboxApp()
{
    CM_LOG_DEBUG("SandboxApp destructor");
    if (m_quad)
    {
        delete m_quad;
    }
}

void SandboxApp::onStart()
{
    m_camera.setPerspective(glm::radians(45.0f), 0.1f, 400.0f);

    Vertex data[] = {
        {{-0.5f, -0.5f, 0.0f}},
        {{ 0.5f, -0.5f, 0.0f}},
        {{ 0.5f,  0.5f, 0.0f}},
        {{-0.5f,  0.5f, 0.0f}}
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    m_quad = new comet::Mesh(data, sizeof(data) / sizeof(data[0]),
                            (const unsigned int*)indices, sizeof(indices) / sizeof(indices[0]));
    // m_quad->setMeshMaterial(&m_blueColorMaterial);

    // m_terrain = new comet::Mesh(data, sizeof(data) / sizeof(data[0]), (const unsigned int*)indices, sizeof(indices) / sizeof(indices[0]));
    // auto transform = glm::rotate(glm::mat4(1.0f), 3.1415f / 4.0f, glm::vec3(1.0f, 0.0f, 0.0f));

    auto scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.7f));

    for (uint32_t i = 0; i < 10; ++i)
    {
        for (uint32_t j = 0; j < 10; ++j)
        {
            auto transform = glm::translate(glm::mat4(1.0f), glm::vec3((j + 1) * -0.9f, (i + 1) * 0.9f, 0.0f));
            auto& instance = m_quad->createMeshInstance();
            instance.setModelTransform(transform * scale);

            if (i == j)
            {
                instance.setMaterial(&m_redColorMaterial);
            }
            else if (i % 2 == 0)
            {
                instance.setMaterial(&m_orangeColorMaterial);
            }
        }
    }

    m_renderer.addMesh(m_quad);
    // m_renderer.addMesh(m_terrain);
    m_renderer.allocateBuffersAndSetupLayouts();
    m_renderer.loadData();

    // m_camera.lookAt(glm::vec3{0.0f, 0.0f, -50.0f}, glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{0.0f, 1.0f, 0.0f});
    m_camera.setPosition(glm::vec3{10.0f, 10.0f, -30.0f});
}

bool SandboxApp::onKeyPressed(comet::KeyPressedEvent& e)
{
}

bool SandboxApp::onVerticalMouseWheelScrolled(comet::VerticalMouseWheelScrolledEvent& e)
{
    m_camera.translate(glm::vec3(0.0f, 0.0f, -e.getDelta() * 0.9f));
}

void SandboxApp::onUpdate(double deltaTime)
{
    const float yawSpeed = 0.001f;
    const float pitchSpeed = 0.001f;
    const float moveSpeed = 0.03f;
    // const float rotSpeed = 0.0015f;

    // m_angle += rotSpeed * deltaTime;
    // m_camera.setPosition(glm::vec3(cos(m_angle) * 40.0f, 0.0f, -sin(m_angle) * 40.0f));
    // m_camera.lookAt(glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{0.0f, 1.0f, 0.0f});

    if (comet::Input::isKeyPressed(comet::Input::Key::A))
    {
        m_camera.translate(glm::vec3(-moveSpeed * deltaTime, 0.0f, 0.0f));
    }
    else if (comet::Input::isKeyPressed(comet::Input::Key::D))
    {
        m_camera.translate(glm::vec3(moveSpeed * deltaTime, 0.0f, 0.0f));
    }

    if (comet::Input::isKeyPressed(comet::Input::Key::S))
    {
        m_camera.translate(glm::vec3(0.0f, moveSpeed * deltaTime, 0.0f));
    }
    else if (comet::Input::isKeyPressed(comet::Input::Key::W))
    {
        m_camera.translate(glm::vec3(0.0f, -moveSpeed * deltaTime, 0.0f));
    }

    if (comet::Input::isKeyPressed(comet::Input::Key::Left))
    {
        auto yaw = m_camera.getYaw();
        yaw -= yawSpeed * deltaTime;
        m_camera.setYaw(yaw);
    }
    else if (comet::Input::isKeyPressed(comet::Input::Key::Right))
    {
        auto yaw = m_camera.getYaw();
        yaw += yawSpeed * deltaTime;
        m_camera.setYaw(yaw);
    }

    if (comet::Input::isKeyPressed(comet::Input::Key::Up))
    {
        auto pitch = m_camera.getPitch();
        pitch -= pitchSpeed * deltaTime;
        m_camera.setPitch(pitch);
    }
    else if (comet::Input::isKeyPressed(comet::Input::Key::Down))
    {
        auto pitch = m_camera.getPitch();
        pitch += pitchSpeed * deltaTime;
        m_camera.setPitch(pitch);
    }

    // for (auto& meshInstance : m_quad->getMeshInstances())
    // {
    //     glm::mat4& transform = meshInstance.getModelTransform();
    //     transform = glm::rotate(transform, (float)deltaTime * 0.0031415f, glm::vec3(0.0f, 0.0f, 1.0f));
        // auto tr = glm::rotate(glm::mat4(1.0f), 0.0031415f, glm::vec3(0.0f, 0.0f, 1.0f));
        // meshInstance.setModelTransform( tr * transform );
    // }
    // m_renderer.prepareBuffers();
}

void SandboxApp::onFixedUpdate(float fixedDeltaTime)
{
}

void SandboxApp::onRender()
{
    m_renderer.render(m_camera);
}