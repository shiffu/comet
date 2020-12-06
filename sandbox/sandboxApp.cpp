#include <glad/glad.h>

#include "sandboxApp.h"
#include <string>
#include <sstream>
#include <math.h>
#include <utility>
#include <memory>
#include <comet/input.h>
#include <comet/vertex.h>
#include <imgui/imgui.h>

comet::Application* comet::Application::getInstance()
{
    static SandboxApp* instance = new SandboxApp();
    return instance;
}

SandboxApp::~SandboxApp()
{
    CM_LOG_DEBUG("SandboxApp destructor");
}

void SandboxApp::onStart()
{
    m_camera.setPerspective(glm::radians(45.0f), 0.1f, 400.0f);
    m_camera.setPosition(glm::vec3{0.0f, 5.0f, 17.0f});

    Vertex data[] = {
        {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
        {{ 0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
        {{ 0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        {{-0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}}
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    // Create the lights
    // Directional Light
    m_directionalLight = std::make_unique<comet::DirectionalLight>(glm::vec3(1.0f, -0.55f, -0.3f));
    m_directionalLight->setDiffuse({0.8f, 0.8f, 0.8f});
    m_directionalLight->setSpecular({0.9f, 0.9f, 0.9f});
    m_renderer.addLight(m_directionalLight.get());

    // Point Lights
    m_pointLights.emplace_back(std::make_unique<comet::PointLight>(glm::vec3(0.37f, 0.19f, 2.9f)));
    m_pointLights.back()->setDiffuse({0.0f, 0.0f, 1.0f});
    m_renderer.addLight(m_pointLights.back().get());

    m_pointLights.emplace_back(std::make_unique<comet::PointLight>(glm::vec3(-3.6f, 0.19f, 0.7f)));
    m_pointLights.back()->setDiffuse({1.0f, 0.0f, 0.0f});
    m_renderer.addLight(m_pointLights.back().get());

    // Spot Lights
    m_spotLights.emplace_back(std::make_unique<comet::SpotLight>(glm::vec3(0.0f, 2.0f, 0.0f)));
    m_spotLights.back()->setDiffuse({1.0f, 0.0f, 1.0f});
    m_renderer.addLight(m_spotLights.back().get());

    m_spotLights.emplace_back(std::make_unique<comet::SpotLight>(glm::vec3(-3.6f, 1.19f, 0.7f)));
    m_spotLights.back()->setDiffuse({1.0f, 1.0f, 0.0f});
    m_renderer.addLight(m_spotLights.back().get());

    // Create Meshes
    m_cube = std::make_unique<comet::Mesh>("torus.obj");
    m_quad = std::make_unique<comet::Mesh>(data, sizeof(data) / sizeof(data[0]),
                            (const unsigned int*)indices, sizeof(indices) / sizeof(indices[0]));
    
    // Set Materials
    // m_phongMaterial.setAlbedoTexture("checkerboard.png");
    m_phongMaterial.setDiffuse(glm::vec3(0.9));
    m_phongMaterial.setSpecular(glm::vec3(1.0f));
    m_phongMaterial.setShininess(50.0f);

    m_cube->setMeshMaterial(&m_phongMaterial);
    m_quad->setMeshMaterial(&m_phongMaterial);

    // Set Instances data
    auto& cubeInstance = m_cube->createMeshInstance();
    cubeInstance.move(glm::vec3(0.0f, 1.0f, -10.0f));
    cubeInstance.scale(1.7f);

    auto& cubeInstance2 = m_cube->createMeshInstance();
    cubeInstance2.move(glm::vec3(5.0f, 1.0f, -10.0f));
    cubeInstance2.scale(1.5f);

    static comet::PhongMaterial phongMaterial2;
    phongMaterial2.setAlbedoTexture("checkerboard.png");
    cubeInstance2.setMaterial(&phongMaterial2);

    const float translationStep = 1.05f;
    const float xOffset = -10.0f;
    const float yOffset = -5.0f;
    
    for (uint32_t i = 0; i < 20; ++i)
    {
        for (uint32_t j = 0; j < 20; ++j)
        {
            auto& instance = m_quad->createMeshInstance();
            instance.rotate(1.5f * glm::pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f));
            instance.move(glm::vec3(xOffset + j * translationStep, yOffset + i * translationStep, 0.0f));
        }
    }

    m_renderer.addMesh(m_cube.get());
    m_renderer.addMesh(m_quad.get());

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

void ligtColorEdit(const char* label, comet::Light* light)
{
    {
        // Ambient
        std::stringstream ss;
        auto ambientColor = light->getAmbient();
        float ambientData[] = {ambientColor.x, ambientColor.y, ambientColor.z};
        ss << label << " Light Ambient Color";
        ImGui::ColorEdit3(ss.str().c_str(), ambientData);
        light->setAmbient({ambientData[0], ambientData[1], ambientData[2]});
    }

    {
        // Diffuse
        std::stringstream ss;
        auto diffuseColor = light->getDiffuse();
        float diffuseData[] = {diffuseColor.x, diffuseColor.y, diffuseColor.z};
        ss << label << " Light Diffuse Color";
        ImGui::ColorEdit3(ss.str().c_str(), diffuseData);
        light->setDiffuse({diffuseData[0], diffuseData[1], diffuseData[2]});
    }
    
    {
        // Specular
        std::stringstream ss;
        auto specularColor = light->getSpecular();
        float specularData[] = {specularColor.x, specularColor.y, specularColor.z};
        ss << label << " Light Specular Color";
        ImGui::ColorEdit3(ss.str().c_str(), specularData);
        light->setSpecular({specularData[0], specularData[1], specularData[2]});
    }
}

void SandboxApp::onImGuiDraw()
{
    ImGui::Begin("SandBox Debug");
    
    // Dragon
    {
        ImGui::Text("Material Attributes");
        auto diffuseColor = m_phongMaterial.getDiffuse();
        float colorData[] = {diffuseColor.x, diffuseColor.y, diffuseColor.z};
        ImGui::ColorEdit3("Diffuse Color", colorData);
        m_phongMaterial.setDiffuse({colorData[0], colorData[1], colorData[2]});
    }
    
    {
        auto specularColor = m_phongMaterial.getSpecular();
        float colorData[] = {specularColor.x, specularColor.y, specularColor.z};
        ImGui::ColorEdit3("Specular Color", colorData);
        m_phongMaterial.setSpecular({colorData[0], colorData[1], colorData[2]});
    }
    
    {
        int shininess = m_phongMaterial.getShininess();
        ImGui::SliderInt("Shininess", &shininess, 0, 128);
        m_phongMaterial.setShininess(shininess);
    }
    
    // Directional Light
    {
        ImGui::Separator();
        ImGui::Text("Directional Light Attributes");
        auto directionalLightPos = m_directionalLight->getDirection();
        float pos[] = {directionalLightPos.x, directionalLightPos.y, directionalLightPos.z};
        ImGui::SliderFloat3("Dir Light Direction", pos, -1.0f, 1.0f);
        m_directionalLight->setDirection({pos[0], pos[1], pos[2]});
    }
    
    ligtColorEdit("Dir", m_directionalLight.get());

    // Point Light(s)
    static int pointLightSelected{0};
    ImGui::Separator();
    ImGui::Text("Point Light Attributes");
    if (ImGui::RadioButton("Select Pt Light 0", pointLightSelected == 0)) { pointLightSelected = 0; } ImGui::SameLine();
    if (ImGui::RadioButton("Select Pt Light 1", pointLightSelected == 1)) { pointLightSelected = 1; }

    auto currentPointLight = m_pointLights[pointLightSelected].get();
    {
        auto lightPos = currentPointLight->getPosition();
        float pos[] = {lightPos.x, lightPos.y, lightPos.z};
        ImGui::SliderFloat3("Point Light Position", pos, -20.0f, 20.0f);
        currentPointLight->setPosition({pos[0], pos[1], pos[2]});
    }

    ligtColorEdit("Point", currentPointLight);

    // Spot Light(s)
    static int spotLightSelected{0};
    ImGui::Separator();
    ImGui::Text("Spot Light Attributes");
    if (ImGui::RadioButton("Select Spot Light 0", spotLightSelected == 0)) { spotLightSelected = 0; } ImGui::SameLine();
    if (ImGui::RadioButton("Select Spot Light 1", spotLightSelected == 1)) { spotLightSelected = 1; }

    auto currentSpotLight = m_spotLights[spotLightSelected].get();
    {
        auto lightPos = currentSpotLight->getPosition();
        float pos[] = {lightPos.x, lightPos.y, lightPos.z};
        ImGui::SliderFloat3("Spot Light Position", pos, -20.0f, 20.0f);
        currentSpotLight->setPosition({pos[0], pos[1], pos[2]});
    }
    
    {
        auto lightDir = currentSpotLight->getDirection();
        float dir[] = {lightDir.x, lightDir.y, lightDir.z};
        ImGui::SliderFloat3("Spot Light Direction", dir, -1.0f, 1.0f);
        currentSpotLight->setDirection({dir[0], dir[1], dir[2]});
    }
    
    ligtColorEdit("Spot", currentSpotLight);

    ImGui::End();
}

void SandboxApp::onUpdate(double deltaTime)
{
    static const float yawSpeed = glm::radians(360.0) / 3000.0f;
    static const float pitchSpeed = glm::radians(360.0) / 3000.0f;
    static const float rollSpeed = glm::radians(360.0) / 3000.0f;
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
        m_cube->getMeshInstances()[0]->rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f));
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