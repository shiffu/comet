#pragma once

#include <panels/sceneHierarchyPanel.h>
#include <panels/propertiesPanel.h>

#include <comet/comet.h>

#include <memory>

namespace comet
{

    class CometEditorApp : public comet::Application
    {
    public:

        CometEditorApp() { CM_LOG_DEBUG("CometEditorApp contructor"); } ;
        virtual ~CometEditorApp();

        virtual void onStart() override;
        virtual void onImGuiInit() override;
        virtual void onImGuiDraw() override;
        virtual void onUpdate(double deltaTime) override;

        virtual void onPreRenderScene() override;
        virtual void onPostRenderScene() override;

        virtual void onEvent(comet::Event& e) override;
        virtual bool onKeyPressed(comet::KeyPressedEvent& e) override;

    private:
        comet::Camera m_camera;
        comet::CameraController m_cameraController{m_camera};
        std::unique_ptr<DirectionalLight> m_directionalLight{};
        std::unique_ptr<Framebuffer> m_frameBuffer;
        SceneHierarchyPanel m_sceneHierarchyPanel;
        PropertiesPanel m_propertiesPanel;
    };

}
