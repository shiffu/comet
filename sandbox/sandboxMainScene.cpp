#include "sandboxMainScene.h"

#include <comet/comet.h>

#include <glm/glm.hpp>


class TestScript : public comet::NativeScript
{
    static constexpr float rotSpeed = glm::radians(360.0) / 5000.0f;

public:
    TestScript() = default;
    virtual ~TestScript() = default;

    virtual void onUpdate(comet::Entity& entity, double deltaTime) override
    {
        auto& transformComponent = getComponent<comet::TransformComponent>(entity);
        float angle = 0.001f * deltaTime;
        transformComponent.rotation.x += angle;
    }
};

bool SandboxMainScene::onEvent(comet::Event& e)
{
    m_cameraController.onEvent(e);
}

bool SandboxMainScene::onKeyPressed(comet::KeyPressedEvent& e)
{
    if (e.getKeyCode() == comet::Input::Key::Space)
    {
        m_pauseAnimation = !m_pauseAnimation;
    }
}

void SandboxMainScene::onStart()
{
    m_camera.setPerspective(glm::radians(45.0f), 16.0f/9.0f, 0.1f, 400.0f);
    m_cameraController.setPosition(glm::vec3{0.0f, 5.0f, 17.0f});

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
    auto material = comet::MaterialRegistry::getInstance().createMaterialInstance("material1");
    auto materialInstanceId = material->getInstanceId();
    material->setDiffuse({0.8f, 0.0f, 0.0f});
    material->setSpecular(glm::vec3(1.0f));
    material->setShininess(10.0f);

    auto material2 = comet::MaterialRegistry::getInstance().createMaterialInstance("checkerboardMaterial");
    auto materialInstanceId2 = material2->getInstanceId();
    material2->setAlbedoTexture("checkerboard.png");
    material2->setDiffuse(glm::vec3(0.9));
    material2->setSpecular(glm::vec3(1.0f));
    material2->setShininess(50.0f);

    // Entities
    for (int i = 0; i < 200; ++i)
    {
        comet::Entity e = createEntity();
        auto meshComp = e.addComponent<comet::MeshComponent>(meshHandler.resourceId);
        e.addComponent<comet::MaterialComponent>(materialInstanceId);
        auto& transformComp = e.getComponent<comet::TransformComponent>();
        transformComp.translation = glm::vec3((i * 2)  % 20, 2.0f + (i / 100), -5 + ((i / 20) * 3) % 10);
        if (i % 15)
            e.addComponent<comet::NativeScriptComponent>().bind<TestScript>();
    }

    const float translationStep = 1.05f;
    const float xOffset = -5.0f;
    const float zOffset = -5.0f;
    
    for (uint32_t i = 0; i < 20; ++i)
    {
        for (uint32_t j = 0; j < 20; ++j)
        {
            auto e = createEntity();
            auto meshComp = e.addComponent<comet::MeshComponent>(meshGroundHandler.resourceId);
            e.addComponent<comet::MaterialComponent>(materialInstanceId2);
            auto& transformComp = e.getComponent<comet::TransformComponent>();

            transformComp.rotation = glm::vec3(-0.5f * glm::pi<float>(), 0.0f, 0.0f);
            transformComp.translation = glm::vec3(xOffset + j * translationStep, 0.0f, zOffset + i * translationStep);
        }
    }

    // Create the lights
    // Directional Light
    m_directionalLight = std::make_unique<comet::DirectionalLight>(glm::vec3(1.0f, -0.55f, -0.3f));
    m_directionalLight->setDiffuse({0.8f, 0.8f, 0.8f});
    m_directionalLight->setSpecular({0.9f, 0.9f, 0.9f});
    addLight(m_directionalLight.get());

    // Point Lights
    m_pointLights.emplace_back(std::make_unique<comet::PointLight>(glm::vec3(0.37f, 0.19f, 2.9f)));
    m_pointLights.back()->setDiffuse({0.0f, 0.0f, 1.0f});
    addLight(m_pointLights.back().get());

    m_pointLights.emplace_back(std::make_unique<comet::PointLight>(glm::vec3(-3.6f, 0.19f, 0.7f)));
    m_pointLights.back()->setDiffuse({1.0f, 0.0f, 0.0f});
    addLight(m_pointLights.back().get());

    // Spot Lights
    m_spotLights.emplace_back(std::make_unique<comet::SpotLight>(glm::vec3(0.0f, 2.0f, 0.0f)));
    m_spotLights.back()->setDiffuse({1.0f, 0.0f, 1.0f});
    addLight(m_spotLights.back().get());

    m_spotLights.emplace_back(std::make_unique<comet::SpotLight>(glm::vec3(-3.6f, 1.19f, 0.7f)));
    m_spotLights.back()->setDiffuse({1.0f, 1.0f, 0.0f});
    addLight(m_spotLights.back().get());
}

void SandboxMainScene::onRender()
{
    m_renderer.reloadInstanceData();
    m_renderer.render(m_cameraController.getView(), m_cameraController.getProjection());
}
