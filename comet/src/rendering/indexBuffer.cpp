#include <glad/glad.h>
#include <utility>
#include <type_traits>
#include <comet/indexBuffer.h>

namespace comet
{
    IndexBuffer::IndexBuffer(uint32_t usage)
        : Buffer(GL_ELEMENT_ARRAY_BUFFER, usage)
    {
    }

    void IndexBuffer::allocate()
    {
        Buffer::allocate();
    }

    IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other) noexcept
    {
        Buffer::operator=(std::move(other));
        return *this;
    }

    void IndexBuffer::allocate(uint32_t indexCount)
    {
        m_size = indexCount * sizeof(uint32_t);
        Buffer::allocate(m_size);
    }

    void IndexBuffer::loadDataInMappedMemory(const uint32_t* data, uint32_t indexCount, uint32_t offset /*= 0*/)
    {
        Buffer::loadDataInMappedMemory((const void*)data, indexCount * sizeof(uint32_t), indexCount, offset);
    }
}