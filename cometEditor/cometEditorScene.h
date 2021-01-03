#pragma once

#include <comet/scene.h>

#include <panels/sceneHierarchyPanel.h>
#include <panels/propertiesPanel.h>

#include <memory>

namespace comet
{

    class Framebuffer;
    class DirectionalLight;

    class CometEditorScene : public Scene
    {
    public:
        CometEditorScene() = default;
        virtual ~CometEditorScene() = default;

        virtual void onBeginRender() override;
        virtual void onEndRender() override;
        
        virtual bool onEvent(Event& e) override;

        virtual void onStart() override;

        virtual void onImGuiDraw() override;

    private:
        void doImGuiInit();

    private:
        comet::Camera m_camera;
        comet::CameraController m_cameraController{m_camera};
        std::unique_ptr<Framebuffer> m_frameBuffer;
        std::unique_ptr<DirectionalLight> m_directionalLight{};
        SceneHierarchyPanel m_sceneHierarchyPanel;
        PropertiesPanel m_propertiesPanel;
    };

} // namespace comet
