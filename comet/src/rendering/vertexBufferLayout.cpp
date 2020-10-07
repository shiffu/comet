#include <comet/vertexBufferLayout.h>
#include <glad/glad.h>

namespace comet
{

    template <>
    void VertexBufferLayout::add<float>(unsigned int count, bool normalized)
    {
        m_attributes.emplace_back(GL_FLOAT, count, sizeof(GLfloat), normalized);
        m_stride += count * sizeof(float);
    }

    template <>
    void VertexBufferLayout::add<unsigned int>(unsigned int count, bool normalized)
    {
        m_attributes.emplace_back(GL_UNSIGNED_INT, count, sizeof(GLuint), normalized);
        m_stride += count * sizeof(GLuint);
    }

} // namespace comet
