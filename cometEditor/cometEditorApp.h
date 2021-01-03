#pragma once

#include <comet/comet.h>

#include "cometEditorScene.h"

namespace comet
{

    class CometEditorApp : public Application
    {
    public:
        CometEditorApp() { CM_LOG_DEBUG("CometEditorApp contructor"); } ;
        virtual ~CometEditorApp();

        virtual void onStart() override;

    private:
        CometEditorScene m_editorScene;
    };

}
