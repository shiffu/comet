#include "cometGameScene.h"

#include <comet/framebuffer.h>
#include <comet/components.h>
#include <core/sceneSerializer.h>
#include <comet/components.h>
#include <comet/renderPass.h>

#include <glm/glm.hpp>

namespace comet
{

    void CometGameScene::onStart()
    {
        FramebufferSpec spec{1280, 720};
        spec.attachmentSet = {FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::DEPTH24_STENCIL8};
        spec.swapChainTarget = true;
        auto frameBuffer = Framebuffer::create(spec);

        RenderPassSpec baseRenderPassSpec{ frameBuffer };
        addRenderPass(baseRenderPassSpec, Renderer(this));

        // TODO: Default main scene should be retrieved from the project config
        const char* mainScene = "EditorScene.scn";
        if (!SceneSerializer::deserialize(*this, mainScene))
        {
            CM_CORE_LOG_ERROR("Unable to start scene {}", mainScene);
        }

        auto directionalLight = std::make_unique<DirectionalLight>(glm::vec3(1.0f, -0.70f, -0.1f));
        directionalLight->setDiffuse({0.8f, 0.8f, 0.8f});
        directionalLight->setSpecular({0.9f, 0.9f, 0.9f});
        addLight(std::move(directionalLight));
    }

    void CometGameScene::onStop() {}
    bool CometGameScene::onEvent(Event& e) {}

    glm::mat4 CometGameScene::getViewMatrix()
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

    glm::mat4 CometGameScene::getProjectionMatrix()
    {
        glm::mat4 cameraProjection{1.0f};

        // Find the primary camera
        m_registry.view<CameraComponent>()
                  .each([this, &cameraProjection](auto entityId, auto& cameraComponent)
        {
            if (cameraComponent.isPrimary)
            {
                Entity entity{entityId, this};
                cameraProjection = cameraComponent.camera.getProjection();
            }
        });

        return cameraProjection;
    }

} // namespace comet
