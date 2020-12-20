#pragma once

#include <comet/log.h>

#include <vector>

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
        VertexBufferLayout() = default;
        virtual ~VertexBufferLayout() {};

        static std::unique_ptr<VertexBufferLayout> create();

        virtual uint32_t getStride() const = 0;
        virtual const std::vector<VertexAttribute>& getAttributes() const = 0;
        
        virtual void addFloat(uint32_t count, bool normalized, uint32_t loc, uint32_t divisor = 0) = 0;
        virtual void addUInt(uint32_t count, bool normalized, uint32_t loc, uint32_t divisor = 0) = 0;
        virtual void addInt(uint32_t count, bool normalized, uint32_t loc, uint32_t divisor = 0) = 0;
    };

} // namespace comet
