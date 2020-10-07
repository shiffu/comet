#pragma once

#include <comet/vertexBuffer.h>
#include <comet/indexBuffer.h>
#include <comet/vertexBufferLayout.h>

namespace comet
{
    class VertexArray
    {
    public:
        VertexArray();
        ~VertexArray();

        void bind() const;
        void unbind() const;
        void addLayout(const VertexBufferLayout& vbl, const VertexBuffer& vbo, const IndexBuffer& ibo);

    private:
        unsigned int m_vao{0};
    };

} // namespace comet
