#pragma once

#include <comet/flatColorMaterial.h>
#include <comet/timer.h>

#include <unordered_map>

namespace comet
{
    class ShaderDrawContext;
    class Light;
    class Scene;

    class Renderer
    {
    public:
        ~Renderer();

        void addLight(Light* light);
        const std::vector<Light*>& getLights() { return m_lights; }

        void allocateBuffersAndSetupLayouts();
        void loadData();
        void reloadInstanceData();
        void render(const glm::mat4& view, const glm::mat4& projection);

        void setScene(Scene* scene);

    private:
        void cleanUp();
        bool isReady() {return m_isReady; }

    private:
        std::unordered_map<uint32_t, ShaderDrawContext*> m_shaderDrawContexts;
        FlatColorMaterial m_defaultColorMaterial{glm::vec4(0.99f, 0.16, 0.97f, 1.0f)};
        std::vector<Light*> m_lights{};
        Scene* m_scene{nullptr};
        bool m_isReady{false};

        Timer T1{"reloadData - T1", true};
        Timer T2{"reloadData - T2", true};
        Timer T3{"reloadData - T3", true};
    };

} // namespace comet