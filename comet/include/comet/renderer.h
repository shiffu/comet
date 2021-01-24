#pragma once

#include <comet/timer.h>

#include <glm/mat4x4.hpp>

#include <unordered_map>

namespace comet
{
    class ShaderDrawContext;
    class Material;
    class Light;
    class Scene;

    class Renderer
    {
    public:
        Renderer();
        ~Renderer();

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
        Material* m_defaultMaterial{nullptr};
        Scene* m_scene{nullptr};
        bool m_isReady{false};

        Timer T1{"reloadData - T1", true};
        Timer T2{"reloadData - T2", true};
        Timer T3{"reloadData - T3", true};
    };

} // namespace comet