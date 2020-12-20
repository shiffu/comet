#pragma once

#include <rendering/indexBuffer.h>

namespace comet
{

    class OpenglIndexBuffer : public IndexBuffer
    {
    public:
        OpenglIndexBuffer(uint32_t usage);
        ~OpenglIndexBuffer();

        OpenglIndexBuffer(const OpenglIndexBuffer&) = delete;
        void operator=(const OpenglIndexBuffer&) = delete;

        OpenglIndexBuffer(OpenglIndexBuffer&&);
        OpenglIndexBuffer& operator=(OpenglIndexBuffer&& other) noexcept;

        virtual void bind() const override;
        virtual void unbind() const override;

        // allocate with the size defined by setSize()
        virtual void allocate() override;
        virtual void allocate(size_t size) override;

        virtual void loadDataInMappedMemory(const uint32_t* data, uint32_t count, uint32_t offset = 0) override;
        virtual void* mapMemory(uint32_t access) override;
        virtual void unmapMemory() override;

        virtual void setSize(size_t size) override { m_size = size; }
        virtual void increaseSize(size_t size) override { m_size += size; }
        virtual uint32_t getCount() const override;
        virtual size_t getSize() const override { return m_size; }

    protected:
        uint32_t m_usage;
        uint32_t m_bufferId{0};
        size_t m_size{0};
        uint32_t m_count{0};
        void* m_pMappedMemory{nullptr};
    };
    
} // namespace comet
