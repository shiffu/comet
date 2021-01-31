#pragma once

#include <comet/scene.h>
#include <comet/directionalLight.h>

#include <memory>

namespace comet
{

    class CometGameScene : public Scene
    {
    public:
        CometGameScene(const char* name) : Scene(name) {}
        virtual ~CometGameScene() = default;

        virtual bool onEvent(Event& e) override;
        virtual void onStart() override;
        virtual void onStop() override;

        virtual glm::mat4 getViewMatrix() override;
        virtual glm::mat4 getProjectionMatrix() override;
    };

} // namespace comet
