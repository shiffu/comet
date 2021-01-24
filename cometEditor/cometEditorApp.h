#pragma once

#include "cometEditorScene.h"
#include "cometGameScene.h"

#include <comet/comet.h>
#include <core/imguiWrapper.h>
#include <panels/sceneHierarchyPanel.h>
#include <panels/propertiesPanel.h>

namespace comet
{

    class CometEditorApp : public Application
    {
    public:
        CometEditorApp() { CM_LOG_DEBUG("CometEditorApp contructor"); } ;
        virtual ~CometEditorApp();

        virtual void onStart() override;
        virtual void onStop() override;
        virtual void onUpdate() override;
        virtual void onRender() override;
        
    private:
        void initImGuiTheme();
        void drawEditor();
        void drawEditorDebug();
        void drawFramebuffer(Scene& scene);

    private:
        CometEditorScene m_editorScene{"CometEditorScene"};
        CometGameScene m_gameScene{"CometGameScene"};
        bool m_isGamePlaying{false};
        std::unique_ptr<ImguiWrapper> m_imguiWrapper;
        SceneHierarchyPanel m_sceneHierarchyPanel;
        PropertiesPanel m_propertiesPanel;
    };

}
