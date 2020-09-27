#pragma once
#include <glad/glad.h>
#include <comet/vertexBuffer.h>
#include <comet/vertexBufferLayout.h>

namespace comet
{
    class VertexArray
    {
    public:
        VertexArray()
        {
            glGenVertexArrays(1, &m_vao);
        }

        ~VertexArray()
        {
            if (m_vao)
            {
                unbind();
                glDeleteVertexArrays(1, &m_vao);
            }
        }

        void bind() const { glBindVertexArray(m_vao); }
        void unbind() const { glBindVertexArray(0); }
        void addLayout(const VertexBuffer& vbo, const VertexBufferLayout& vbl);

    private:
        unsigned int m_vao{0};
    };

} // namespace comet
