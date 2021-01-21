#pragma once

#include "sandboxMainScene.h"

#include <comet/comet.h>

class SandboxApp : public comet::Application
{
public:

    SandboxApp() { CM_LOG_DEBUG("SandboxApp contructor"); } ;
    virtual ~SandboxApp();

    virtual void onStart() override;

private:
    SandboxMainScene m_mainScene{"SandboxMainScene"};
};