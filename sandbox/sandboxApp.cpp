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
    m_quad->setMeshMaterial(&m_blueColorMaterial);

    // m_terrain = new comet::Mesh(data, sizeof(data) / sizeof(data[0]), (const unsigned int*)indices, sizeof(indices) / sizeof(indices[0]));

    auto scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.7f));
    const float translationStep = 0.85f;

    for (uint32_t i = 0; i < 20; ++i)
    {
        for (uint32_t j = 0; j < 20; ++j)
        {
            auto& instance = m_quad->createMeshInstance();
            instance.move(glm::vec3(j * translationStep, i * translationStep, 0.0f));
            instance.scale(0.7f);

            if (i == 0 && j == 0)
            {
                instance.setMaterial(&m_redColorMaterial);
            }
            else if (i % 2 == 0)
            {
                instance.setMaterial(&m_orangeColorMaterial);
                instance.rotate(glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            }
        }
    }

    m_renderer.addMesh(m_quad);
    // m_renderer.addMesh(m_terrain);
    m_renderer.allocateBuffersAndSetupLayouts();
    m_renderer.loadData();
    m_camera.setPosition(glm::vec3{0.0f, 0.0f, 30.0f});
}

bool SandboxApp::onKeyPressed(comet::KeyPressedEvent& e)
{
    auto pos = m_camera.getPosition();
    CM_LOG_DEBUG("Camera pos: {}, {}, {}", pos.x, pos.y, pos.z);
}

bool SandboxApp::onVerticalMouseWheelScrolled(comet::VerticalMouseWheelScrolledEvent& e)
{
    m_camera.moveFront(-e.getDelta() * 0.9f);
}

void SandboxApp::onUpdate(double deltaTime)
{
    static const float yawSpeed = 0.004f;
    static const float pitchSpeed = 0.004f;
    static const float moveSpeed = 0.015f;
    static const float rotSpeed = glm::radians(360.0) / 1000.0f;

    if (comet::Input::isKeyPressed(comet::Input::Key::R))
    {
        m_camera.lookAt(glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{0.0f, 1.0f, 0.0f});
    }

    if (comet::Input::isKeyPressed(comet::Input::Key::A))
    {
        m_camera.moveRight(-moveSpeed * deltaTime);
    }
    else if (comet::Input::isKeyPressed(comet::Input::Key::D))
    {
        m_camera.moveRight(moveSpeed * deltaTime);
    }

    if (comet::Input::isKeyPressed(comet::Input::Key::S))
    {
        m_camera.moveUp(-moveSpeed * deltaTime);
    }
    else if (comet::Input::isKeyPressed(comet::Input::Key::W))
    {
        m_camera.moveUp(moveSpeed * deltaTime);
    }

    if (comet::Input::isKeyPressed(comet::Input::Key::Left))
    {
        m_camera.addYaw(yawSpeed * deltaTime);
    }
    else if (comet::Input::isKeyPressed(comet::Input::Key::Right))
    {
        m_camera.addYaw(-yawSpeed * deltaTime);
    }

    if (comet::Input::isKeyPressed(comet::Input::Key::Up))
    {
        m_camera.addPitch(pitchSpeed * deltaTime);
    }
    else if (comet::Input::isKeyPressed(comet::Input::Key::Down))
    {
        m_camera.addPitch(-pitchSpeed * deltaTime);
    }

    auto& instances = m_quad->getMeshInstances();
    instances[0].rotate(rotSpeed * deltaTime, glm::vec3(0.0f, 0.0f, 1.0f));

    m_renderer.reloadInstanceData();
}

void SandboxApp::onFixedUpdate(float fixedDeltaTime)
{
}

void SandboxApp::onRender()
{
    m_renderer.render(m_camera);
}