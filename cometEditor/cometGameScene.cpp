#include "cometGameScene.h"

#include <comet/framebuffer.h>
#include <comet/components.h>
#include <core/sceneSerializer.h>
#include <comet/components.h>

#include <glm/glm.hpp>

namespace comet
{

    void CometGameScene::onStart()
    {
        m_frameBuffer = Framebuffer::create({1280, 720});
        m_frameBuffer->invalidate();

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
    
    void CometGameScene::onRender()
    {
        glm::mat4 cameraProjection;
        glm::mat4 cameraView;
        bool primaryCameraFound{false};

        // Find the primary camera
        m_registry.view<CameraComponent>()
                  .each([this, &cameraView, &cameraProjection, &primaryCameraFound](auto entityId, auto& scriptComponent)
        {
            if (scriptComponent.isPrimary)
            {
                Entity entity{entityId, this};
                cameraProjection = scriptComponent.camera.getProjection();
                cameraView = glm::inverse(entity.getComponent<TransformComponent>().getTransform());
                primaryCameraFound = true;
            }
        });

        if (primaryCameraFound)
        {
            m_frameBuffer->bind();

            m_renderer.reloadInstanceData();
            m_renderer.render(cameraView, cameraProjection);
            
            m_frameBuffer->unbind();
        }
    }

} // namespace comet
