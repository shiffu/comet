#include "cometEditorApp.h"
#include <core/imguiWrapper.h>

#include <imgui/imgui.h>

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
        m_imguiWrapper = ImguiWrapper::create();
        if (m_imguiWrapper)
        {
            m_imguiWrapper->init();
            m_imguiWrapper->initPlatform(getWindow());
        }

        setActiveScene(&m_editorScene);
    }

    void CometEditorApp::onStop()
    {
        if (m_imguiWrapper)
        {
            m_imguiWrapper->shutdown();
            m_imguiWrapper = nullptr;
        }
    }

    void CometEditorApp::onUpdate()
    {
        if (m_imguiWrapper)
        {
            m_imguiWrapper->newFrame();
            m_editorScene.onImGuiDraw();
        }
    }

    void CometEditorApp::onRender()
    {
        if (m_imguiWrapper)
        {
            m_imguiWrapper->render();
        }
    }
}
