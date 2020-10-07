#pragma once

#include <vector>
#include <comet/log.h>

namespace comet
{
    struct VertexAttribute
    {
        unsigned int type;
        unsigned int count;
        unsigned int size;
        bool normalized;

        VertexAttribute(unsigned int pType, unsigned int pCount, unsigned int pSize, bool pNormalized)
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
        void add(unsigned int count, bool normalized = false);

    private:
        std::vector<VertexAttribute> m_attributes;
        unsigned int m_stride;
    };

    template <>
    void VertexBufferLayout::add<float>(unsigned int count, bool normalized);

    template <>
    void VertexBufferLayout::add<unsigned int>(unsigned int count, bool normalized);

} // namespace comet
