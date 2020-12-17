#pragma once
#include <glad/glad.h>
#include <unordered_map>
#include <comet/flatColorMaterial.h>
#include <comet/timer.h>

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

        void addScene(Scene* scene);

    private:
        std::unordered_map<uint32_t, ShaderDrawContext*> m_shaderDrawContexts;
        FlatColorMaterial m_defaultColorMaterial{glm::vec4(0.99f, 0.16, 0.97f, 1.0f)};
        std::vector<Light*> m_lights{};
        Scene* m_scene{nullptr};
        Timer t1{"T1", true};
        Timer t2{"T2", true};
    };

} // namespace comet