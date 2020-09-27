#pragma once

#include <comet/vertexBuffer.h>
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
        void addLayout(const VertexBuffer& vbo, const VertexBufferLayout& vbl);

    private:
        unsigned int m_vao{0};
    };

} // namespace comet
