#include <glad/glad.h>

#include "sandboxApp.h"
#include <comet/comet.h>
#include <imgui/imgui.h>
#include <string>
#include <sstream>
#include <memory>

comet::Application* comet::Application::getInstance()
{
    static auto instance = std::make_unique<SandboxApp>();
    return instance.get();
}

SandboxApp::~SandboxApp()
{
    CM_LOG_DEBUG("SandboxApp destructor");
}

void SandboxApp::onStart()
{
    m_camera.setPerspective(glm::radians(45.0f), 16.0f/9.0f, 0.1f, 400.0f);
    m_cameraController.setPosition(glm::vec3{0.0f, 5.0f, 17.0f});
    getActiveScene().setCameraController(&m_cameraController);

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


    // Meshes
    auto meshHandler = comet::ResourceManager::getInstance().loadStaticMesh("torus.obj");
    auto meshGroundHandler = comet::ResourceManager::getInstance().createStaticMesh("groundTiles", data, sizeof(data) / sizeof(data[0]),
                                                (const unsigned int*)indices, sizeof(indices) / sizeof(indices[0]));

    // Materials
    auto material = comet::MaterialRegistry::getInstance().createMaterial<comet::PhongMaterial>();
    auto materialInstanceId = material->getInstanceId();
    auto materialTypeHash = material->getTypeHash();

    auto material2 = comet::MaterialRegistry::getInstance().createMaterial<comet::PhongMaterial>();
    auto materialInstanceId2 = material2->getInstanceId();
    auto materialTypeHash2 = material2->getTypeHash();
    material2->setAlbedoTexture("checkerboard.png");
    material2->setDiffuse(glm::vec3(0.9));
    material2->setSpecular(glm::vec3(1.0f));
    material2->setShininess(50.0f);

    // Entities
    comet::Entity e1 = getActiveScene().createEntity();
    auto meshComp = e1.addComponent<comet::MeshComponent>(meshHandler.resourceId, materialTypeHash, materialInstanceId);
    auto& transformComp = e1.getComponent<comet::TransformComponent>();
    transformComp.move(glm::vec3(2.0f));

    for (int i = 0; i < 2500; ++i)
    {
        comet::Entity e = getActiveScene().createEntity();
        auto meshComp = e.addComponent<comet::MeshComponent>(meshHandler.resourceId, materialTypeHash, materialInstanceId);
        auto& transformComp = e.getComponent<comet::TransformComponent>();
        transformComp.move(glm::vec3((i * 2)  % 25, 1.0f + (i) % 25, 1.0f - (i / 20)  % 100));
    }

    const float translationStep = 1.05f;
    const float xOffset = -10.0f;
    const float yOffset = -5.0f;
    
    for (uint32_t i = 0; i < 50; ++i)
    {
        for (uint32_t j = 0; j < 50; ++j)
        {
            auto e = getActiveScene().createEntity();
            auto meshComp = e.addComponent<comet::MeshComponent>(meshGroundHandler.resourceId, materialTypeHash2, materialInstanceId2);
            auto& transformComp = e.getComponent<comet::TransformComponent>();

            transformComp.rotate(1.5f * glm::pi<float>(), glm::vec3(1.0f, 0.0f, 0.0f));
            transformComp.move(glm::vec3(xOffset + j * translationStep, yOffset + i * translationStep, 0.0f));
        }
    }

    // Create the lights
    // Directional Light
    m_directionalLight = std::make_unique<comet::DirectionalLight>(glm::vec3(1.0f, -0.55f, -0.3f));
    m_directionalLight->setDiffuse({0.8f, 0.8f, 0.8f});
    m_directionalLight->setSpecular({0.9f, 0.9f, 0.9f});
    getActiveScene().addLight(m_directionalLight.get());

    // Point Lights
    m_pointLights.emplace_back(std::make_unique<comet::PointLight>(glm::vec3(0.37f, 0.19f, 2.9f)));
    m_pointLights.back()->setDiffuse({0.0f, 0.0f, 1.0f});
    getActiveScene().addLight(m_pointLights.back().get());

    m_pointLights.emplace_back(std::make_unique<comet::PointLight>(glm::vec3(-3.6f, 0.19f, 0.7f)));
    m_pointLights.back()->setDiffuse({1.0f, 0.0f, 0.0f});
    getActiveScene().addLight(m_pointLights.back().get());

    // Spot Lights
    m_spotLights.emplace_back(std::make_unique<comet::SpotLight>(glm::vec3(0.0f, 2.0f, 0.0f)));
    m_spotLights.back()->setDiffuse({1.0f, 0.0f, 1.0f});
    getActiveScene().addLight(m_spotLights.back().get());

    m_spotLights.emplace_back(std::make_unique<comet::SpotLight>(glm::vec3(-3.6f, 1.19f, 0.7f)));
    m_spotLights.back()->setDiffuse({1.0f, 1.0f, 0.0f});
    getActiveScene().addLight(m_spotLights.back().get());

    // Prepare the Scene
    getActiveScene().prepare();
}

void SandboxApp::onEvent(comet::Event& e)
{
    m_cameraController.onEvent(e);
}

bool SandboxApp::onKeyPressed(comet::KeyPressedEvent& e)
{
    if (e.getKeyCode() == comet::Input::Key::Space)
    {
        m_pauseAnimation = !m_pauseAnimation;
    }
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
    static const float rotSpeed = glm::radians(360.0) / 5000.0f;

    if (!m_pauseAnimation)
    {
        auto angle = rotSpeed * deltaTime;
        getActiveScene().getRegistry().group<comet::TransformComponent, comet::MeshComponent>().each([&](auto& transform, auto& mesh)
        {
            transform.rotate(angle, glm::vec3(1.0f, 0.0f, 0.0f));
        });
    }
}
