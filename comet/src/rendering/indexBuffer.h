#pragma once

#include <rendering/buffer.h>
#include <memory>

namespace comet
{
    class IndexBuffer : public Buffer
    {
    public:
        virtual ~IndexBuffer() {};

        virtual void loadDataInMappedMemory(const uint32_t* data, uint32_t count, uint32_t offset = 0) = 0;

        static std::unique_ptr<IndexBuffer> create(uint32_t usage);
    };
}