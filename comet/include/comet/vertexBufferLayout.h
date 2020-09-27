#pragma once
#include <glad/glad.h>
#include <vector>
#include <comet/log.h>

namespace comet
{
    struct VertexAttribute
    {
        GLenum type;
        GLint count;
        unsigned int size;
        GLboolean normalized;

        VertexAttribute(GLenum pType, GLint pCount, unsigned int pSize, GLboolean pNormalized)
            : type(pType), count(pCount), size(pSize), normalized(pNormalized)
            {
                CM_CORE_LOG_DEBUG("VertexAttribute: type = {}, count = {}, size = {}", type, count, size);
            }
    };

    class VertexBufferLayout
    {
    public:
        VertexBufferLayout() : m_stride(0){};
        ~VertexBufferLayout(){};

        unsigned int getStride() const { return m_stride; }
        const std::vector<VertexAttribute>& getAttributes() const { return m_attributes; }

        template <typename T>
        void add(GLint count, GLboolean normalized = GL_FALSE);

    private:
        std::vector<VertexAttribute> m_attributes;
        unsigned int m_stride;
    };

    template <>
    void VertexBufferLayout::add<float>(GLint count, GLboolean normalized);

    template <>
    void VertexBufferLayout::add<unsigned int>(GLint count, GLboolean normalized);

} // namespace comet

