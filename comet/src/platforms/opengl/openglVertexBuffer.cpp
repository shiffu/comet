#include "openglVertexBuffer.h"
#include <comet/log.h>

#include <glad/glad.h>

namespace comet
{

    OpenglVertexBuffer::OpenglVertexBuffer(uint32_t usage)
        : m_usage(usage)
    {
        glGenBuffers(1, &m_bufferId);
    }

    OpenglVertexBuffer::~OpenglVertexBuffer()
    {
        if (m_bufferId)
        {
            unbind();
            glDeleteBuffers(1, &m_bufferId);
        }
    };

    OpenglVertexBuffer::OpenglVertexBuffer(OpenglVertexBuffer&& other)
        : m_usage(std::move(other.m_usage)),
        m_bufferId(std::move(other.m_bufferId)),
        m_size(std::move(other.m_size)),
        m_count(std::move(other.m_count)),
        m_pMappedMemory(std::move(other.m_pMappedMemory))
    {
        other.m_bufferId = 0;
    }

    OpenglVertexBuffer& OpenglVertexBuffer::operator=(OpenglVertexBuffer&& other) noexcept
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

    uint32_t OpenglVertexBuffer::getCount() const
    {
        return m_count;
    }

    void OpenglVertexBuffer::bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_bufferId);
    }

    void OpenglVertexBuffer::unbind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void OpenglVertexBuffer::allocate()
    {
        if (m_size)
        {
            bind();
            glBufferData(GL_ARRAY_BUFFER, m_size, nullptr, m_usage);
        }
    }

    void OpenglVertexBuffer::allocate(size_t size)
    {
        bind();
        m_size = size;
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, m_usage);
    }

    void OpenglVertexBuffer::loadDataInMappedMemory(const void* data, size_t size, uint32_t count, uint32_t offset /*= 0*/)
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

    void* OpenglVertexBuffer::mapMemory(uint32_t access)
    {
        bind();
        m_pMappedMemory = glMapBuffer(GL_ARRAY_BUFFER, (GLenum)access);
        return m_pMappedMemory;
    }

    void OpenglVertexBuffer::unmapMemory()
    {
        bind();
        if (!glUnmapBuffer(GL_ARRAY_BUFFER))
        {
            CM_CORE_LOG_ERROR("Error while unmapping OpenglVertexBuffer");
        }
        m_pMappedMemory = nullptr;
    }

} // namespace comet
