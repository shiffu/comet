#pragma once
#include <cstdint>
#include <comet/buffer.h>
#include <comet/vertex.h>

namespace comet
{
    class VertexBuffer : public Buffer
    {
    public:
        VertexBuffer(uint32_t usage);
        ~VertexBuffer() {};

        VertexBuffer& operator=(VertexBuffer&& other) noexcept;
    };
}