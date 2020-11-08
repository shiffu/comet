#pragma once
#include <glad/glad.h>
#include <unordered_map>
#include <comet/mesh.h>
#include <comet/camera.h>

namespace comet
{
    class DrawingContext;

    class Renderer
    {
    public:
        void addMesh(Mesh* mesh);
        
        void createAllocateBuffers();
        void loadData();
        void render(const Camera& camera);

    private:
        std::unordered_map<std::string, DrawingContext*> m_drawingContexts;
    };

} // namespace comet