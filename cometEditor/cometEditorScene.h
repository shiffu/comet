#pragma once

#include <comet/scene.h>
#include <comet/cameraController.h>
#include <comet/directionalLight.h>
#include <comet/framebuffer.h>

#include <memory>

namespace comet
{

    class CometEditorScene : public Scene
    {
    public:
        CometEditorScene(const char* name) : Scene(name) {}
        virtual ~CometEditorScene() = default;

        virtual void onRender() override;
        virtual bool onEvent(Event& e) override;
        virtual void onStart() override;

        Framebuffer& getFramebuffer() { return *m_frameBuffer.get(); }

    private:
        Camera m_camera;
        CameraController m_cameraController{m_camera};
        std::unique_ptr<Framebuffer> m_frameBuffer;
        std::unique_ptr<DirectionalLight> m_directionalLight{};
    };

} // namespace comet
