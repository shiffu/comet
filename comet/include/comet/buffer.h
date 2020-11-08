#pragma once
#include <glad/glad.h>
#include <cstdint>

namespace comet
{

    class Buffer
    {
    public:
        Buffer(uint32_t target, uint32_t usage);
        ~Buffer();

        Buffer(const Buffer&) = delete;
        Buffer(Buffer&&);
        void operator=(const Buffer&) = delete;
        Buffer& operator=(Buffer&& other) noexcept;

        void bind() const;
        void unbind() const;

        void setTarget(uint32_t target) { m_target = target; }

        // allocate with the size defined by setSize()
        void allocate();
        void allocate(uint32_t size);
        void loadDataInMappedMemory(const void* data, uint32_t size, uint32_t count, uint32_t offset = 0);
        void* mapMemory(GLenum access);
        void unmapMemory();

        inline void setSize(uint32_t size) { m_size = size; }
        inline void increaseSize(uint32_t size) { m_size += size; }
        inline uint32_t getSize() const { return m_size; }
        inline uint32_t getCurrentCount() const { return m_currentCount; }

    protected:
        uint32_t m_target;
        uint32_t m_usage;
        uint32_t m_bufferId{0};
        uint32_t m_size{0};
        uint32_t m_currentCount{0};
        void* m_pMappedMemory{nullptr};
    };
}