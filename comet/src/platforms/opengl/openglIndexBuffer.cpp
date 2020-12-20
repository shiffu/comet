#include "openglIndexBuffer.h"
#include <comet/log.h>

#include <glad/glad.h>

namespace comet
{

    OpenglIndexBuffer::OpenglIndexBuffer(uint32_t usage)
        : m_usage(usage)
    {
        glGenBuffers(1, &m_bufferId);
    }

    OpenglIndexBuffer::~OpenglIndexBuffer()
    {
        if (m_bufferId)
        {
            unbind();
            glDeleteBuffers(1, &m_bufferId);
        }
    };

    OpenglIndexBuffer::OpenglIndexBuffer(OpenglIndexBuffer&& other)
        : m_usage(std::move(other.m_usage)),
        m_bufferId(std::move(other.m_bufferId)),
        m_size(std::move(other.m_size)),
        m_count(std::move(other.m_count)),
        m_pMappedMemory(std::move(other.m_pMappedMemory))
    {
        other.m_bufferId = 0;
    }

    OpenglIndexBuffer& OpenglIndexBuffer::operator=(OpenglIndexBuffer&& other) noexcept
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

    uint32_t OpenglIndexBuffer::getCount() const
    {
        return m_count;
    }

    void OpenglIndexBuffer::bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_bufferId);
    }

    void OpenglIndexBuffer::unbind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void OpenglIndexBuffer::allocate()
    {
        if (m_size)
        {
            bind();
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_size, nullptr, m_usage);
        }
    }

    void OpenglIndexBuffer::allocate(size_t size)
    {
        bind();
        m_size = size;
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr, m_usage);
    }

    void OpenglIndexBuffer::loadDataInMappedMemory(const uint32_t* data, uint32_t count, uint32_t offset /*= 0*/)
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

        auto size = count * sizeof(uint32_t);
        memcpy(m_pMappedMemory, data, size);
        m_pMappedMemory = static_cast<char*>(m_pMappedMemory) + size;
        m_count += count;
    }

    void* OpenglIndexBuffer::mapMemory(uint32_t access)
    {
        bind();
        m_pMappedMemory = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, (GLenum)access);
        return m_pMappedMemory;
    }

    void OpenglIndexBuffer::unmapMemory()
    {
        bind();
        if (!glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER))
        {
            CM_CORE_LOG_ERROR("Error while unmapping OpenglIndexBuffer");
        }
        m_pMappedMemory = nullptr;
    }

    
} // namespace comet
