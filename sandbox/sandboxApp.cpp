#include "sandboxApp.h"

#include <comet/comet.h>

#include <memory>

comet::Application* comet::Application::getInstance()
{
    static auto instance = std::make_unique<SandboxApp>();
    return instance.get();
}

SandboxApp::~SandboxApp()
{
    CM_LOG_DEBUG("SandboxApp destructor");
}

void SandboxApp::onStart()
{
    setActiveScene(&m_mainScene);
}
