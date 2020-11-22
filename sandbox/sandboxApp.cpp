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
    m_camera.setPosition(glm::vec3{0.0f, 5.0f, 15.0f});

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


    // Create test Meshes
    m_cube = new comet::Mesh("dragon.obj");
    // m_test = new comet::Mesh("torus.obj");
    m_quad = new comet::Mesh(data, sizeof(data) / sizeof(data[0]),
                            (const unsigned int*)indices, sizeof(indices) / sizeof(indices[0]));
    // m_terrain = new comet::Mesh(data, sizeof(data) / sizeof(data[0]), (const unsigned int*)indices, sizeof(indices) / sizeof(indices[0]));
    
    // Set Materials
    m_phongMaterial.setDiffuse(glm::vec3(0.8f, 0.4f, 0.0f));
    m_phongMaterial.setSpecular(glm::vec3(1.0f, 0.5f, 0.0f));
    m_phongMaterial.setShininess(25.0f);

    m_cube->setMeshMaterial(&m_phongMaterial);
    // m_test->setMeshMaterial(&m_phongMaterial);
    m_quad->setMeshMaterial(&m_blueColorMaterial);

    // Set Instances data
    auto& cubeInstance = m_cube->createMeshInstance();
    cubeInstance.move(glm::vec3(0.0f, 0.0f, 0.0f));
    cubeInstance.scale(0.7f);

    // auto& testInstance1 = m_test->createMeshInstance();
    // testInstance1.move(glm::vec3(-1.0f, -1.0f, -1.0f));

    // auto& testInstance2 = m_test->createMeshInstance();
    // testInstance2.move(glm::vec3(-1.0f, 0.0f, 0.0f));

    const float translationStep = 1.0f;
    const float xOffset = -10.0f;
    const float yOffset = -2.0f;
    for (uint32_t i = 0; i < 20; ++i)
    {
        for (uint32_t j = 0; j < 20; ++j)
        {
            auto& instance = m_quad->createMeshInstance();
            instance.rotate(1.5f * glm::pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f));
            instance.move(glm::vec3(xOffset + j * translationStep, yOffset + i * translationStep, -1.0f));

            if (i % 2 == 0)
            {
                instance.setMaterial(&m_lightBlueColorMaterial);
            }
        }
    }

    m_renderer.addMesh(m_cube);
    // m_renderer.addMesh(m_test);
    m_renderer.addMesh(m_quad);
    // m_renderer.addMesh(m_terrain);

    m_renderer.allocateBuffersAndSetupLayouts();
    m_renderer.loadData();
}

bool SandboxApp::onKeyPressed(comet::KeyPressedEvent& e)
{
    auto pos = m_camera.getPosition();
    CM_LOG_DEBUG("Camera pos: {}, {}, {}", pos.x, pos.y, pos.z);

    if (e.getKeyCode() == comet::Input::Key::Space)
    {
        m_pauseAnimation = !m_pauseAnimation;
    }

}

bool SandboxApp::onVerticalMouseWheelScrolled(comet::VerticalMouseWheelScrolledEvent& e)
{
    m_camera.moveFront(-e.getDelta() * 0.9f);
}

void SandboxApp::onUpdate(double deltaTime)
{
    static const float yawSpeed = glm::radians(360.0) / 2000.0f;
    static const float pitchSpeed = glm::radians(360.0) / 2000.0f;
    static const float rollSpeed = glm::radians(360.0) / 2000.0f;
    static const float moveSpeed = 0.015f;
    static const float rotSpeed = glm::radians(360.0) / 15000.0f;

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

    if (comet::Input::isKeyPressed(comet::Input::Key::Z))
    {
        m_camera.addRoll(rollSpeed * deltaTime);
    }
    else if (comet::Input::isKeyPressed(comet::Input::Key::X))
    {
        m_camera.addRoll(-rollSpeed * deltaTime);
    }

    if (!m_pauseAnimation)
    {
        auto angle = rotSpeed * deltaTime;
        m_cube->getMeshInstances()[0].rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f));
    }

    m_renderer.reloadInstanceData();
}

void SandboxApp::onFixedUpdate(float fixedDeltaTime)
{
}

void SandboxApp::onRender()
{
    m_renderer.render(m_camera);
}