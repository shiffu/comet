#pragma once
#include <cstdint>
#include <comet/buffer.h>

namespace comet
{
    class IndexBuffer : public Buffer
    {
    public:
        IndexBuffer(uint32_t usage);
        ~IndexBuffer() {};

        IndexBuffer& operator=(IndexBuffer&& other) noexcept;

        void allocate();
        void allocate(uint32_t indexCount);
        void loadDataInMappedMemory(const uint32_t* data, uint32_t indexCount, uint32_t offset = 0);
    };
}