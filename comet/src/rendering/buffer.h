#pragma once

#include <cstdint>
#include <cstddef>

namespace comet
{

    class Buffer
    {
    public:
        Buffer() = default;
        virtual ~Buffer() {}

        Buffer(const Buffer&) = delete;
        void operator=(const Buffer&) = delete;

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        // allocate with the size defined by setSize()
        virtual void allocate() = 0;
        virtual void allocate(size_t size) = 0;

        virtual void* mapMemory(uint32_t access) = 0;
        virtual void unmapMemory() = 0;

        virtual void setSize(size_t size) = 0;
        virtual size_t getSize() const = 0;
        virtual uint32_t getCount() const = 0;
        virtual void increaseSize(size_t size) = 0;
    };
    
}