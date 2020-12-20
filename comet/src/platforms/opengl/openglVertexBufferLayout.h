#pragma once

#include <rendering/vertexBufferLayout.h>

namespace comet
{
    
    class OpenglVertexBufferLayout : public VertexBufferLayout
    {
    public:
        OpenglVertexBufferLayout() : m_stride(0) {};
        virtual ~OpenglVertexBufferLayout() {};

        virtual uint32_t getStride() const override;
        virtual const std::vector<VertexAttribute>& getAttributes() const override;

        virtual void addFloat(uint32_t count, bool normalized, uint32_t loc, uint32_t divisor = 0) override;
        virtual void addUInt(uint32_t count, bool normalized, uint32_t loc, uint32_t divisor = 0) override;
        virtual void addInt(uint32_t count, bool normalized, uint32_t loc, uint32_t divisor = 0) override;

    private:
        std::vector<VertexAttribute> m_attributes;
        uint32_t m_stride;
    };

} // namespace comet
