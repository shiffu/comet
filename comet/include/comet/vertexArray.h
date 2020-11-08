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

        VertexArray(const VertexArray&) = delete;
        VertexArray(VertexArray&& other);
        void operator=(const VertexArray&) = delete;
        VertexArray& operator=(VertexArray&& other) noexcept;

        void bind() const;
        void unbind() const;
        void addLayout(const VertexBufferLayout& vbl, const VertexBuffer& vbo, const IndexBuffer* ibo = nullptr);

    private:
        unsigned int m_vao{0};
    };

} // namespace comet