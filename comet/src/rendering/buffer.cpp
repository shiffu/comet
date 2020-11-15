#include <type_traits>
#include <utility>
#include <comet/buffer.h>
#include <comet/log.h>

namespace comet
{
    Buffer::Buffer(uint32_t target, uint32_t usage)
        : m_target(target), m_usage(usage)
    {
        glGenBuffers(1, &m_bufferId);
    }

    Buffer::~Buffer()
    {
        if (m_bufferId)
        {
            unbind();
            glDeleteBuffers(1, &m_bufferId);
        }
    };

    Buffer::Buffer(Buffer&& other)
        : m_target(std::move(other.m_target)),
        m_usage(std::move(other.m_usage)),
        m_bufferId(std::move(other.m_bufferId)),
        m_size(std::move(other.m_size)),
        m_currentCount(std::move(other.m_currentCount)),
        m_pMappedMemory(std::move(other.m_pMappedMemory))
    {
        other.m_bufferId = 0;
    }

    Buffer& Buffer::operator=(Buffer&& other) noexcept
    {
        if (&other == this)
        {
            return *this;
        }
        if (m_bufferId)
        {
            unbind();
            glDeleteBuffers(1, &m_bufferId);
        }

        m_target = std::move(other.m_target);
        m_usage = std::move(other.m_usage);
        m_bufferId = std::move(other.m_bufferId);
        m_size = std::move(other.m_size);
        m_currentCount = std::move(other.m_currentCount);
        m_pMappedMemory = std::move(other.m_pMappedMemory);

        other.m_bufferId = 0;

        return *this;
    }

    void Buffer::bind() const
    {
        glBindBuffer(m_target, m_bufferId);
    }

    void Buffer::unbind() const
    {
        glBindBuffer(m_target, 0);
    }

    void Buffer::allocate()
    {
        if (m_size)
        {
            bind();
            glBufferData(m_target, m_size, nullptr, m_usage);
        }
    }

    void Buffer::allocate(uint32_t size)
    {
        bind();
        m_size = size;
        glBufferData(m_target, size, nullptr, m_usage);
    }

    void Buffer::loadDataInMappedMemory(const void* data, uint32_t size, uint32_t count, uint32_t offset /*= 0*/)
    {
        if (m_pMappedMemory == nullptr)
        {
            CM_CORE_LOG_ERROR("To load data in mapped memory, you need to map memory first by calling 'mapMemory'");
            return;
        }

        if (offset)
        {
            m_pMappedMemory = static_cast<char*>(m_pMappedMemory) + offset;
        }

        memcpy(m_pMappedMemory, data, size);
        m_pMappedMemory = static_cast<char*>(m_pMappedMemory) + size;
        m_currentCount += count;
    }

    void* Buffer::mapMemory(GLenum access)
    {
        bind();
        m_pMappedMemory = glMapBuffer(m_target, access);
        return m_pMappedMemory;
    }

    void Buffer::unmapMemory()
    {
        bind();
        if (!glUnmapBuffer(m_target))
        {
            CM_CORE_LOG_ERROR("Error while unmapping Buffer");
        }
        m_pMappedMemory = nullptr;
    }
}