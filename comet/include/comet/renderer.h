#pragma once

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
        Renderer(Scene* scene);
        ~Renderer();
        
        void prepare();
        void reloadInstanceData();
        void render();

    private:
        void initFromScene();
        void allocateBuffersAndSetupLayouts();
        void loadDataToBuffers();
        void cleanUp();

    private:
        std::unordered_map<uint32_t, ShaderDrawContext*> m_shaderDrawContexts;
        Material* m_defaultMaterial{nullptr};
        Scene* m_scene{nullptr};

        Timer reloadInstanceData_T1{"reloadInstanceData - T1", true};
        Timer reloadInstanceData_T2{"reloadInstanceData - T2", true};
        Timer reloadInstanceData_T3{"reloadInstanceData - T3", true};
    };

} // namespace comet