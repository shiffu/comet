#include "cometEditorScene.h"

#include <comet/framebuffer.h>
#include <comet/directionalLight.h>
#include <comet/resourceManager.h>
#include <comet/materialRegistry.h>
#include <comet/components.h>
#include <comet/nativeScript.h>

#include <glm/glm.hpp>

namespace comet
{

    void CometEditorScene::onStart()
    {
        m_camera.setPerspective(glm::radians(45.0f), 16.0f/9.0f, 0.1f, 400.0f);
        m_cameraController.setPosition(glm::vec3{0.0f, 5.0f, 20.0f});

        m_frameBuffer = Framebuffer::create({1280, 720});
        m_frameBuffer->invalidate();

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
    
    void CometEditorScene::onRender()
    {
        m_frameBuffer->bind();

        m_renderer.reloadInstanceData();
        m_renderer.render(m_cameraController.getView(), m_cameraController.getProjection());
        
        m_frameBuffer->unbind();
    }

} // namespace comet
