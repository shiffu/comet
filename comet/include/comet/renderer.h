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
        Renderer() = default;
        virtual ~Renderer() = default;
        
        virtual Renderer& operator=(const Renderer&& other) {}

        virtual void prepare() = 0;
        virtual void reloadData() = 0;
        virtual void render() = 0;
    };

    class SceneRenderer : public Renderer
    {
    public:
        using PreRenderFunc = void (*)(SceneRenderer& renderer, void* userData);

        SceneRenderer() = default;
        SceneRenderer(Scene* scene);
        virtual ~SceneRenderer();

        // SceneRenderer(SceneRenderer&& other)
        //     : m_shaderDrawContexts(std::move(other.m_shaderDrawContexts)),
        //     m_defaultMaterial(std::move(other.m_defaultMaterial)),
        //     m_scene(std::move(other.m_scene)),
        //     m_preRenderFunction(std::move(other.m_preRenderFunction)),
        //     m_userData(std::move(other.m_userData)),
        //     m_view(std::move(other.m_view)),
        //     m_projection(std::move(other.m_projection)),
        //     reloadInstanceData_T1(std::move(other.reloadInstanceData_T1)),
        //     reloadInstanceData_T2(std::move(other.reloadInstanceData_T2)),
        //     reloadInstanceData_T3(std::move(other.reloadInstanceData_T3)) {}

        // SceneRenderer& operator=(const SceneRenderer&& other);
        
        void prepare() override;
        void reloadData() override;
        void render() override;

        void registerPreRenderCallback(PreRenderFunc func, void* userData)
        {
            m_preRenderFunction = func;
            m_userData = userData;
        }

        void setViewMatrix(const glm::mat4& view) { m_view = view; }
        void setProjectionMatrix(const glm::mat4& projection) { m_projection = projection; }

    private:
        void initFromScene();
        void allocateBuffersAndSetupLayouts();
        void loadDataToBuffers();
        void cleanUp();

    private:
        std::unordered_map<uint32_t, ShaderDrawContext*> m_shaderDrawContexts;
        Material* m_defaultMaterial{nullptr};
        Scene* m_scene{nullptr};

        PreRenderFunc m_preRenderFunction{nullptr};
        void* m_userData{nullptr};

        glm::mat4 m_view{1.0f};
        glm::mat4 m_projection{1.0f};

        Timer reloadInstanceData_T1{"reloadData - Entities processing", true};
        Timer reloadInstanceData_T2{"reloadData - Data Load in GPU", true};
        Timer reloadInstanceData_T3{"reloadData - Total", true};
    };

} // namespace comet