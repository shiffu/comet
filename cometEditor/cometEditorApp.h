#pragma once

#include "cometEditorScene.h"

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
        void doImGuiInit();
        void drawImGui();
        void drawImGuiDebug();

    private:
        CometEditorScene m_editorScene{"CometEditorScene"};
        std::unique_ptr<ImguiWrapper> m_imguiWrapper;
        SceneHierarchyPanel m_sceneHierarchyPanel;
        PropertiesPanel m_propertiesPanel;
    };

}
