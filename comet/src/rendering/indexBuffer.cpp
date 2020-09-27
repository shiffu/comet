#include <comet/indexBuffer.h>
#include <glad/glad.h>
#include <type_traits>

namespace comet
{
    IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count) : m_count(count)
    {
        static_assert(sizeof(unsigned int) == sizeof(GLuint), "GLuint has not the same size has an unsigned int");
        glGenBuffers(1, &m_ibo);
        bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
    };

    IndexBuffer::~IndexBuffer()
    {
        if (m_ibo)
        {
            unbind();
            glDeleteBuffers(1, &m_ibo);
        }
    };

    void IndexBuffer::bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
    }

    void IndexBuffer::unbind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}