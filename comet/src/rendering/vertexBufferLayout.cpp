#include <comet/vertexBufferLayout.h>

namespace comet
{

    template <>
    void VertexBufferLayout::add<float>(GLint count, GLboolean normalized)
    {
        m_attributes.emplace_back(GL_FLOAT, count, sizeof(GLfloat), normalized);
        m_stride += count * sizeof(GLfloat);
    }

    template <>
    void VertexBufferLayout::add<unsigned int>(GLint count, GLboolean normalized)
    {
        m_attributes.emplace_back(GL_UNSIGNED_INT, count, sizeof(GLuint), normalized);
        m_stride += count * sizeof(GLuint);
    }

} // namespace comet
