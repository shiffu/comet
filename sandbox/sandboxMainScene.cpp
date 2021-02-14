#include "sandboxMainScene.h"

#include <comet/comet.h>

#include <glm/glm.hpp>

using namespace comet;


class TestScript : public NativeScript
{
    static constexpr float rotSpeed = glm::radians(360.0) / 5000.0f;

public:
    TestScript() = default;
    virtual ~TestScript() = default;

    virtual void onUpdate(Entity& entity, double deltaTime) override
    {
        auto& transformComponent = getComponent<TransformComponent>(entity);
        float angle = 0.001f * deltaTime;
        transformComponent.rotation.x += angle;
    }
};

bool SandboxMainScene::onEvent(Event& e)
{
    EventDispatcher dispatcher(e);

    const float zoomSpeed = 0.01f;
    dispatcher.dispatch<VerticalMouseWheelScrolledEvent>([this, &zoomSpeed](VerticalMouseWheelScrolledEvent& e) -> bool {
        m_camera.increaseZoom(-e.getDelta() * zoomSpeed);
        
        return true;
    });
}

bool SandboxMainScene::onKeyPressed(KeyPressedEvent& e)
{
    if (e.getKeyCode() == Input::Key::Space)
    {
        m_pauseAnimation = !m_pauseAnimation;
    }
}

glm::mat4 SandboxMainScene::getViewMatrix()
{
    glm::mat4 cameraView{1.0f};

    // Find the primary camera
    m_registry.view<CameraComponent>()
                .each([this, &cameraView](auto entityId, auto& cameraComponent)
    {
        if (cameraComponent.isPrimary)
        {
            Entity entity{entityId, this};
            cameraView = glm::inverse(entity.getComponent<TransformComponent>().getTransform());
        }
    });

    return cameraView;
}

void SandboxMainScene::onUpdate(double deltaTime)
{
    const float moveSpeed = 0.05f;

    if (Input::isKeyPressed(Input::Key::A))
    {
        auto& cameraTransform = m_cameraEntity.getComponent<TransformComponent>();
        cameraTransform.translation -= glm::vec3(moveSpeed, 0.0f, 0.0f);
    }
    else if (Input::isKeyPressed(Input::Key::D))
    {
        auto& cameraTransform = m_cameraEntity.getComponent<TransformComponent>();
        cameraTransform.translation += glm::vec3(moveSpeed, 0.0f, 0.0f);
    }
}

void SandboxMainScene::onStart()
{
    m_camera.setPerspective(glm::radians(65.0f), 16.0f/9.0f, 0.1f, 500.0f);

    // Camera entity
    m_cameraEntity = createEntity();
    m_cameraEntity.addComponent<CameraComponent>(m_camera);
    auto& cameraTransform = m_cameraEntity.getComponent<TransformComponent>();
    cameraTransform.translation = {0.0f, 12.0f, 13.0f};
    cameraTransform.rotation = {glm::radians(-25.0f), 0.0f, 0.0f};

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
    auto meshHandler = ResourceManager::getInstance().loadStaticMesh("torus.obj");
    auto meshGroundHandler = ResourceManager::getInstance().createStaticMesh("groundTiles", data, sizeof(data) / sizeof(data[0]),
                                                (const unsigned int*)indices, sizeof(indices) / sizeof(indices[0]));

    // Materials
    auto material = MaterialRegistry::getInstance().createMaterialInstance("material1");
    auto materialInstanceId = material->getInstanceId();
    material->setDiffuse({0.8f, 0.0f, 0.0f});
    material->setSpecular(glm::vec3(1.0f));
    material->setShininess(10.0f);

    auto material2 = MaterialRegistry::getInstance().createMaterialInstance("checkerboardMaterial");
    auto materialInstanceId2 = material2->getInstanceId();
    material2->setAlbedoTexture("checkerboard.png");
    material2->setDiffuse(glm::vec3(0.9));
    material2->setSpecular(glm::vec3(1.0f));
    material2->setShininess(50.0f);

    // Entities
    for (uint32_t i = 0; i < 8; ++i)
    {
        for (uint32_t j = 0; j < 8; ++j)
        {
            for (uint32_t k = 0; k < 4; ++k)
            {
                Entity e = createEntity();
                auto meshComp = e.addComponent<MeshComponent>(meshHandler.resourceId);
                e.addComponent<MaterialComponent>(materialInstanceId);
                auto& transformComp = e.getComponent<TransformComponent>();
                transformComp.translation = glm::vec3(-10.0f + j * 2.4f, 1.0f + i * 1.7f, -2.5f * k);
                e.addComponent<NativeScriptComponent>().bind<TestScript>();
            }
        }
    }

    const float translationStep = 1.05f;
    const float xOffset = -5.0f;
    const float zOffset = -5.0f;
    
    for (uint32_t i = 0; i < 10; ++i)
    {
        for (uint32_t j = 0; j < 10; ++j)
        {
            auto e = createEntity();
            auto meshComp = e.addComponent<MeshComponent>(meshGroundHandler.resourceId);
            e.addComponent<MaterialComponent>(materialInstanceId2);
            auto& transformComp = e.getComponent<TransformComponent>();

            transformComp.rotation = glm::vec3(-0.5f * glm::pi<float>(), 0.0f, 0.0f);
            transformComp.translation = glm::vec3(xOffset + j * translationStep, 0.0f, zOffset + i * translationStep);
        }
    }

    FramebufferSpec spec{1280, 720};
    spec.attachmentSet = {FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::DEPTH24_STENCIL8};
    spec.swapChainTarget = true;
    auto frameBuffer = Framebuffer::create(spec);

    RenderPassSpec renderPassSpec{ frameBuffer };
    auto renderer = std::make_unique<SceneRenderer>(this);
    renderer->registerPreRenderCallback( [] (SceneRenderer& renderer, void* sceneInstance) {
        SandboxMainScene* self = (SandboxMainScene*)sceneInstance;
        if (self)
        {
            renderer.setViewMatrix(self->getViewMatrix());
            renderer.setProjectionMatrix(self->getProjectionMatrix());
        }
    }, this);
    addRenderPass(renderPassSpec, std::move(renderer));            

    // Create the lights
    // Directional Light
    auto directionalLight = std::make_unique<DirectionalLight>(glm::vec3(1.0f, -0.55f, -0.3f));
    directionalLight->setDiffuse({0.8f, 0.8f, 0.8f});
    directionalLight->setSpecular({0.9f, 0.9f, 0.9f});
    addLight(std::move(directionalLight));

    // Point Lights
    auto pointLight = std::make_unique<PointLight>(glm::vec3(0.37f, 0.19f, 2.9f));
    pointLight->setDiffuse({0.0f, 0.0f, 1.0f});
    addLight(std::move(pointLight));

    // Spot Lights
    auto spotLight = std::make_unique<SpotLight>(glm::vec3(0.0f, 2.0f, 0.0f));
    spotLight->setDiffuse({1.0f, 0.0f, 1.0f});
    addLight(std::move(spotLight));
}
