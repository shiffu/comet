#include "cometEditorScene.h"

#include <comet/renderPass.h>
#include <comet/components.h>
#include <comet/directionalLight.h>

#include <glm/glm.hpp>

namespace comet
{

    glm::mat4 CometEditorScene::getViewMatrixFromScene()
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

    glm::mat4 CometEditorScene::getProjectionMatrixFromScene()
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

    void CometEditorScene::onStart()
    {
        m_camera.setPerspective(glm::radians(45.0f), 16.0f/9.0f, 0.1f, 400.0f);
        m_cameraController.setPosition(glm::vec3{0.0f, 5.0f, 20.0f});

        // RenderPass definitions
        // Main RenderPass
        {
            FramebufferSpec spec{1280, 720};
            spec.attachmentSet = {
                {FramebufferTextureFormat::RGBA8, {0.0f, 0.02f, 0.1f, 1.0f}},
                FramebufferTextureFormat::DEPTH24_STENCIL8
            };
            auto frameBuffer = Framebuffer::create(spec);

            RenderPassSpec renderPassSpec{ frameBuffer };
            auto renderer = std::make_unique<SceneRenderer>(this);
            renderer->registerPreRenderCallback( [] (SceneRenderer& renderer, void* sceneInstance) {
                CometEditorScene* self = (CometEditorScene*)sceneInstance;
                if (self)
                {
                    renderer.setViewMatrix(self->getViewMatrix());
                    renderer.setProjectionMatrix(self->getProjectionMatrix());
                }
            }, this);
            addRenderPass(renderPassSpec, std::move(renderer));            
        }

        // ScenePreview Panel RenderPass
        {
            FramebufferSpec spec{640, 360};
            spec.attachmentSet = {
                FramebufferTextureFormat::RGBA8,
                FramebufferTextureFormat::DEPTH24_STENCIL8
            };
            auto frameBuffer = Framebuffer::create(spec);

            RenderPassSpec renderPassSpec{ frameBuffer };
            auto renderer = std::make_unique<SceneRenderer>(this);
            renderer->registerPreRenderCallback( [] (SceneRenderer& renderer, void* sceneInstance) {
                CometEditorScene* self = (CometEditorScene*)sceneInstance;
                if (self)
                {
                    renderer.setViewMatrix(self->getViewMatrixFromScene());
                    renderer.setProjectionMatrix(self->getProjectionMatrixFromScene());
                }
            }, this);
            addRenderPass(renderPassSpec, std::move(renderer));
        }

        // TODO: Remove this default directional light once Environment is implemented
        auto directionalLight = std::make_unique<DirectionalLight>(glm::vec3(1.0f, -0.70f, -0.1f));
        directionalLight->setDiffuse({0.8f, 0.8f, 0.8f});
        directionalLight->setSpecular({0.9f, 0.9f, 0.9f});
        addLight(std::move(directionalLight));
    }

    bool CometEditorScene::onEvent(Event& e)
    {
        m_cameraController.onEvent(e);
    }

} // namespace comet
