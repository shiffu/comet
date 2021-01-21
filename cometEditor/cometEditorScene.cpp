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
    
    void CometEditorScene::onRender()
    {
        m_frameBuffer->bind();

        m_renderer.reloadInstanceData();
        m_renderer.render(m_cameraController.getView(), m_cameraController.getProjection());
        
        m_frameBuffer->unbind();
    }

    void CometEditorScene::onStart()
    {
        m_camera.setPerspective(glm::radians(45.0f), 16.0f/9.0f, 0.1f, 400.0f);
        m_cameraController.setPosition(glm::vec3{0.0f, 5.0f, 20.0f});

        m_frameBuffer = Framebuffer::create({1280, 720});
        m_frameBuffer->invalidate();

        // TEST SCENE
        auto materialGround = MaterialRegistry::getInstance().createMaterialInstance("grey");
        materialGround->setDiffuse({0.092f, 0.092f, 0.188f});
        materialGround->setSpecular({0.431f, 0.431f, 0.478f});
        materialGround->setShininess(5.5f);

        auto meshHandler = ResourceManager::getInstance().loadStaticMesh("cube.obj");

        auto e1 = createEntity();
        e1.getComponent<NameComponent>().name = "Ground";
        e1.getComponent<TransformComponent>().translation = glm::vec3(0.0f, 0.0f, 0.0f);
        e1.getComponent<TransformComponent>().scale = glm::vec3(20.0f, 0.001f, 20.0f);
        e1.addComponent<MeshComponent>(meshHandler.resourceId);
        e1.addComponent<MaterialComponent>(materialGround->getInstanceId());

        m_directionalLight = std::make_unique<DirectionalLight>(glm::vec3(1.0f, -0.70f, -0.1f));
        m_directionalLight->setDiffuse({0.8f, 0.8f, 0.8f});
        m_directionalLight->setSpecular({0.9f, 0.9f, 0.9f});
        addLight(m_directionalLight.get());
    }

    bool CometEditorScene::onEvent(Event& e)
    {
        m_cameraController.onEvent(e);
    }

} // namespace comet
