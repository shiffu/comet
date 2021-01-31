#include "cometEditorScene.h"

#include <comet/renderPass.h>
#include <comet/directionalLight.h>

#include <glm/glm.hpp>

namespace comet
{

    void CometEditorScene::onStart()
    {
        m_camera.setPerspective(glm::radians(45.0f), 16.0f/9.0f, 0.1f, 400.0f);
        m_cameraController.setPosition(glm::vec3{0.0f, 5.0f, 20.0f});

        FramebufferSpec spec{1280, 720};
        spec.attachmentSet = {FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::DEPTH24_STENCIL8};
        spec.swapChainTarget = true;
        spec.clearColor = {0.0f, 0.1f, 0.3, 1.0f};
        auto frameBuffer = Framebuffer::create(spec);

        RenderPassSpec baseRenderPassSpec{ frameBuffer };
        addRenderPass(baseRenderPassSpec, Renderer(this));

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
