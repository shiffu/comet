#pragma once

#include <comet/comet.h>

class SandboxMainScene : public comet::Scene
{
public:
    virtual void onStart()  override;
    virtual void onShutdown()  override;
    virtual void onUpdate(double deltaTime)  override;
    virtual void onFixedUpdate(float fixedDeltaTime)  override;

private:
};