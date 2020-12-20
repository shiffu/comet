#pragma once

#include <rendering/vertexBuffer.h>
#include <rendering/vertexBufferLayout.h>

namespace comet
{
    class IndexBuffer;

    class VertexArray
    {
    public:
        VertexArray() = default;
        virtual ~VertexArray() {};

        static std::unique_ptr<VertexArray> create();

        VertexArray(const VertexArray&) = delete;
        void operator=(const VertexArray&) = delete;
        
        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        virtual void addLayout(const VertexBufferLayout& vbl, const VertexBuffer& vbo, const IndexBuffer* ibo = nullptr) = 0;
    };

} // namespace comet