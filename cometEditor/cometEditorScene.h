#pragma once

#include <comet/scene.h>
#include <comet/cameraController.h>
#include <comet/directionalLight.h>

namespace comet
{

    class CometEditorScene : public Scene
    {
    public:
        CometEditorScene(const char* name) : Scene(name, false) {}
        virtual ~CometEditorScene() = default;

        virtual bool onEvent(Event& e) override;
        virtual void onStart() override;

        virtual glm::mat4 getViewMatrix() override { return m_cameraController.getView(); }
        virtual glm::mat4 getProjectionMatrix() override { return m_camera.getProjection(); }

    private:
        Camera m_camera;
        CameraController m_cameraController{m_camera};
        std::unique_ptr<DirectionalLight> m_directionalLight{};
    };

} // namespace comet
