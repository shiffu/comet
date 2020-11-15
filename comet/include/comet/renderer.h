#pragma once
#include <glad/glad.h>
#include <unordered_map>
#include <comet/mesh.h>
#include <comet/camera.h>
#include <comet/flatColorMaterial.h>

namespace comet
{
    class ShaderDrawContext;

    class Renderer
    {
    public:
        ~Renderer();

        void addMesh(Mesh* mesh);
        void allocateBuffersAndSetupLayouts();
        void loadData();
        void render(const Camera& camera);

    private:
        std::unordered_map<std::string, ShaderDrawContext*> m_materialDrawContexts;
        FlatColorMaterial m_defaultColorMaterial{glm::vec4(0.99f, 0.16, 0.97f, 1.0f)};
    };

} // namespace comet