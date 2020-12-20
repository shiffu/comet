#include "openglCommandBuffer.h"
#include <comet/log.h>

#include <glad/glad.h>

namespace comet
{

    OpenglCommandBuffer::OpenglCommandBuffer(uint32_t usage)
        : m_usage(usage)
    {
        glGenBuffers(1, &m_bufferId);
    }

    OpenglCommandBuffer::~OpenglCommandBuffer()
    {
        if (m_bufferId)
        {
            unbind();
            glDeleteBuffers(1, &m_bufferId);
        }
    };

    OpenglCommandBuffer::OpenglCommandBuffer(OpenglCommandBuffer&& other)
        : m_usage(std::move(other.m_usage)),
        m_bufferId(std::move(other.m_bufferId)),
        m_size(std::move(other.m_size)),
        m_count(std::move(other.m_count)),
        m_pMappedMemory(std::move(other.m_pMappedMemory))
    {
        other.m_bufferId = 0;
    }

    OpenglCommandBuffer& OpenglCommandBuffer::operator=(OpenglCommandBuffer&& other) noexcept
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

        m_usage = std::move(other.m_usage);
        m_bufferId = std::move(other.m_bufferId);
        m_size = std::move(other.m_size);
        m_count = std::move(other.m_count);
        m_pMappedMemory = std::move(other.m_pMappedMemory);

        other.m_bufferId = 0;

        return *this;
    }

    uint32_t OpenglCommandBuffer::getCount() const
    {
        return m_count;
    }

    void OpenglCommandBuffer::bind() const
    {
        glBindBuffer(GL_DRAW_INDIRECT_BUFFER, m_bufferId);
    }

    void OpenglCommandBuffer::unbind() const
    {
        glBindBuffer(GL_DRAW_INDIRECT_BUFFER, 0);
    }

    void OpenglCommandBuffer::allocate()
    {
        if (m_size)
        {
            bind();
            glBufferData(GL_DRAW_INDIRECT_BUFFER, m_size, nullptr, m_usage);
        }
    }

    void OpenglCommandBuffer::allocate(size_t size)
    {
        bind();
        m_size = size;
        glBufferData(GL_DRAW_INDIRECT_BUFFER, size, nullptr, m_usage);
    }

    void OpenglCommandBuffer::loadDataInMappedMemory(const void* data, size_t size, uint32_t count, uint32_t offset /*= 0*/)
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
        m_count += count;
    }

    void* OpenglCommandBuffer::mapMemory(uint32_t access)
    {
        bind();
        m_pMappedMemory = glMapBuffer(GL_DRAW_INDIRECT_BUFFER, (GLenum)access);
        return m_pMappedMemory;
    }

    void OpenglCommandBuffer::unmapMemory()
    {
        bind();
        if (!glUnmapBuffer(GL_DRAW_INDIRECT_BUFFER))
        {
            CM_CORE_LOG_ERROR("Error while unmapping OpenglCommandBuffer");
        }
        m_pMappedMemory = nullptr;
    }

} // namespace comet
