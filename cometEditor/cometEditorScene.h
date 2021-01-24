#pragma once

#include <comet/scene.h>
#include <comet/cameraController.h>
#include <comet/directionalLight.h>

#include <memory>

namespace comet
{

    class CometEditorScene : public Scene
    {
    public:
        CometEditorScene(const char* name) : Scene(name, false) {}
        virtual ~CometEditorScene() = default;

        virtual void onRender() override;
        virtual bool onEvent(Event& e) override;
        virtual void onStart() override;

    private:
        Camera m_camera;
        CameraController m_cameraController{m_camera};
        std::unique_ptr<DirectionalLight> m_directionalLight{};
    };

} // namespace comet
