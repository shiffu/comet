#include "cometEditorApp.h"

#include <memory>

namespace comet
{

    Application* Application::getInstance()
    {
        static auto instance = std::make_unique<CometEditorApp>();
        return instance.get();
    }

    CometEditorApp::~CometEditorApp()
    {
        CM_LOG_DEBUG("CometEditorApp destructor");
    }

    void CometEditorApp::onStart()
    {
        setActiveScene(&m_editorScene);
    }
}
