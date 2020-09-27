#include <comet/vertexBuffer.h>
#include <glad/glad.h>

namespace comet
{
    VertexBuffer::VertexBuffer(const void* data, unsigned int size)
    {
        glGenBuffers(1, &m_vbo);
        bind();
        //TODO: Manage other type of memory allocation than GL_STATIC_DRAW
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    };

    VertexBuffer::~VertexBuffer()
    {
        if (m_vbo)
        {
            unbind();
            glDeleteBuffers(1, &m_vbo);
        }
    };

    void VertexBuffer::bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    }

    void VertexBuffer::unbind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void VertexBuffer::loadData(const void* data, unsigned int size)
    {
        bind();

        // Allocate the buffer and set the usage type
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);

        // Upload vertices data
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    }
}