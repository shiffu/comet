#pragma once

#include <vector>
#include <comet/log.h>

namespace comet
{
    struct VertexAttribute
    {
        uint32_t type;
        uint32_t count;
        uint32_t size;
        uint32_t loc;
        uint32_t divisor;
        bool normalized;

        VertexAttribute(uint32_t pLoc, uint32_t pType, uint32_t pCount, uint32_t pSize, bool pNormalized, uint32_t divisor)
            : loc(pLoc), type(pType), count(pCount), size(pSize), normalized(pNormalized), divisor(divisor)
            {
            }
    };

    class VertexBufferLayout
    {
    public:
        VertexBufferLayout() : m_stride(0){};
        ~VertexBufferLayout(){};

        uint32_t getStride() const { return m_stride; }
        const std::vector<VertexAttribute>& getAttributes() const { return m_attributes; }

        template <typename T>
        void add(uint32_t count, bool normalized, uint32_t loc, uint32_t divisor = 0);

    private:
        std::vector<VertexAttribute> m_attributes;
        uint32_t m_stride;
    };

    template <>
    void VertexBufferLayout::add<float>(uint32_t count, bool normalized, uint32_t loc, uint32_t divisor);

    template <>
    void VertexBufferLayout::add<unsigned int>(uint32_t count, bool normalized, uint32_t loc, uint32_t divisor);

} // namespace comet
