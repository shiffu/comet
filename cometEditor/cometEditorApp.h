#pragma once

#include <comet/comet.h>

#include "cometEditorScene.h"
#include <core/imguiWrapper.h>

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
        CometEditorScene m_editorScene{"CometEditorScene"};
        std::unique_ptr<ImguiWrapper> m_imguiWrapper;
    };

}
