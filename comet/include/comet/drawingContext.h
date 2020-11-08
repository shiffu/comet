#pragma once

#include <vector>
#include <comet/shader.h>
#include <comet/vertexArray.h>
#include <comet/indexBuffer.h>
#include <comet/vertexBuffer.h>
#include <comet/commandBuffer.h>

namespace comet
{
    struct DrawingContext
    {
        DrawingContext(Shader* _shader) : shader(_shader) {}

        Shader* shader;
        VertexArray vao;
        IndexBuffer ibo{GL_STATIC_DRAW};
        VertexBuffer vbo{GL_STATIC_DRAW};
        VertexBuffer instanceBuffer{GL_DYNAMIC_DRAW};
        CommandBuffer commandBuffer{GL_DYNAMIC_DRAW};
        std::vector<Mesh*> meshes;
    };
    
} // namespace comet
