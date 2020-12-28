#include "openglFramebuffer.h"

#include <comet/assert.h>

#include <glad/glad.h>

namespace comet
{

    OpenglFramebuffer::OpenglFramebuffer(const FramebufferSpec& spec) : Framebuffer(spec)
    {
        glGenFramebuffers(1, &m_bufferId);
    }

    OpenglFramebuffer::~OpenglFramebuffer()
    {
        if (m_bufferId)
        {
            unbind();
            glDeleteFramebuffers(1, &m_bufferId);
        }
    }

    OpenglFramebuffer::OpenglFramebuffer(OpenglFramebuffer&& other)
        : m_bufferId(std::move(other.m_bufferId)),
        m_colorAttachment(std::move(other.m_colorAttachment)),
        m_depthAttachment(std::move(other.m_depthAttachment))
    {
        other.m_bufferId = 0;
    }

    OpenglFramebuffer& OpenglFramebuffer::operator=(OpenglFramebuffer&& other) noexcept
    {
        if (&other == this)
        {
            return *this;
        }
        if (m_bufferId)
        {
            unbind();
            glDeleteFramebuffers(1, &m_bufferId);
        }

        m_bufferId = std::move(other.m_bufferId);
        m_colorAttachment = std::move(other.m_colorAttachment);
        m_depthAttachment = std::move(other.m_depthAttachment);

        other.m_bufferId = 0;

        return *this;
    }

    void OpenglFramebuffer::invalidate()
    {
        // Bind
        glBindFramebuffer(GL_FRAMEBUFFER, m_bufferId);

        // Color attachment
        if (m_colorAttachment)
        {
            glDeleteTextures(1, &m_colorAttachment);
            m_colorAttachment = 0;
        }
        glCreateTextures(GL_TEXTURE_2D, 1, &m_colorAttachment);
        glBindTexture(GL_TEXTURE_2D, m_colorAttachment);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, m_spec.width, m_spec.height);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorAttachment, 0);
        
        // Depth attachment
        if (m_depthAttachment)
        {
            glDeleteTextures(1, &m_depthAttachment);
            m_depthAttachment = 0;
        }
        glCreateTextures(GL_TEXTURE_2D, 1, &m_depthAttachment);
        glBindTexture(GL_TEXTURE_2D, m_depthAttachment);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_spec.width, m_spec.height);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthAttachment, 0);

        // Check Completeness
        ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

        // Unbind
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glViewport(0, 0, m_spec.width, m_spec.height);
    }

    void OpenglFramebuffer::bind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_bufferId);
    }

    void OpenglFramebuffer::unbind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    
} // namespace comet
