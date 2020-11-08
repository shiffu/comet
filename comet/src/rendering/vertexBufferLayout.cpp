#include <comet/vertexBufferLayout.h>
#include <glad/glad.h>

namespace comet
{

    template <>
    void VertexBufferLayout::add<float>(uint32_t count, bool normalized, uint32_t loc, uint32_t divisor /*= 0*/)
    {
        m_attributes.emplace_back(loc, GL_FLOAT, count, sizeof(GLfloat), normalized, divisor);
        m_stride += count * sizeof(float);
    }

    template <>
    void VertexBufferLayout::add<unsigned int>(uint32_t count, bool normalized, uint32_t loc, uint32_t divisor /*= 0*/)
    {
        m_attributes.emplace_back(loc, GL_UNSIGNED_INT, count, sizeof(GLuint), normalized, divisor);
        m_stride += count * sizeof(GLuint);
    }

} // namespace comet
