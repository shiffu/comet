#include "openglVertexBufferLayout.h"

#include <glad/glad.h>

namespace comet
{

    uint32_t OpenglVertexBufferLayout::getStride() const
    {
        return m_stride;
    }

    const std::vector<VertexAttribute>& OpenglVertexBufferLayout::getAttributes() const
    {
        return m_attributes;
    }

    void OpenglVertexBufferLayout::addFloat(uint32_t count, bool normalized, uint32_t loc, uint32_t divisor /*= 0*/)
    {
        m_attributes.emplace_back(loc, GL_FLOAT, count, sizeof(GLfloat), normalized, divisor);
        m_stride += count * sizeof(float);
    }

    void OpenglVertexBufferLayout::addUInt(uint32_t count, bool normalized, uint32_t loc, uint32_t divisor /*= 0*/)
    {
        m_attributes.emplace_back(loc, GL_UNSIGNED_INT, count, sizeof(GLuint), normalized, divisor);
        m_stride += count * sizeof(GLuint);
    }

    void OpenglVertexBufferLayout::addInt(uint32_t count, bool normalized, uint32_t loc, uint32_t divisor /*= 0*/)
    {
        m_attributes.emplace_back(loc, GL_INT, count, sizeof(GLint), normalized, divisor);
        m_stride += count * sizeof(GLint);
    }

} // namespace comet
