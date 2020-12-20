#pragma once

#include <rendering/buffer.h>
#include <memory>

namespace comet
{
    class VertexBuffer : public Buffer
    {
    public:
        virtual ~VertexBuffer() {};

        virtual void loadDataInMappedMemory(const void* data, size_t size, uint32_t count, uint32_t offset = 0) = 0;

        static std::unique_ptr<VertexBuffer> create(uint32_t usage);
    };
}