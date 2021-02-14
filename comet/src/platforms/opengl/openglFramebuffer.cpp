#include "openglFramebuffer.h"

#include <comet/texture.h>
#include <comet/application.h>

#include <glad/glad.h>

namespace comet
{

    // Framebuffer util functions
    static bool isDepthFormat(FramebufferTextureFormat format)
    {
        switch (format)
        {
            // case FramebufferTextureFormat::STENCIL8:
            case FramebufferTextureFormat::DEPTH24_STENCIL8:
            // case FramebufferTextureFormat::DEPTH32F_STENCIL8:
            // case FramebufferTextureFormat::DEPTH32F:
                return true;
                break;
        }
        return false;
    }

    static GLenum getTextureTarget(bool isMultisampled)
    {
        return isMultisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
    }

    static void createTextures(bool isMultisampled, uint32_t* textureIds, size_t count)
    {
        glCreateTextures(getTextureTarget(isMultisampled), count, textureIds);
    }

    static void bindTexture(bool isMultisampled, uint32_t id)
    {
        glBindTexture(getTextureTarget(isMultisampled), id);
    }

    static void attachColorTexture(uint32_t id, uint16_t samples, GLenum format, uint16_t width, uint16_t height, uint16_t index)
    {
        // Multisampled?
        if (samples > 1)
        {
            glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
        }
        else
        {
            glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }
        
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index,
                                getTextureTarget(samples > 1), id, 0);
    }

    static void attachDepthTexture(uint32_t id, uint16_t samples, GLenum format, GLenum attachmentType, uint16_t width, uint16_t height)
    {
        // Multisampled?
        if (samples > 1)
        {
            glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
        }
        else
        {
            glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }
        
        glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, getTextureTarget(samples > 1), id, 0);
    }

    static void attachDepthRenderBuffer(uint32_t id, uint16_t samples, GLenum format, GLenum attachmentType, uint16_t width, uint16_t height)
    {
        glGenRenderbuffers(1, &id);
        glBindRenderbuffer(GL_RENDERBUFFER, id);

        // Multisampled?
        if (samples > 1)
        {
            glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, format, width, height);
        }
        else
        {
            glRenderbufferStorage(GL_RENDERBUFFER, format, width, height);
        }

        glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachmentType, GL_RENDERBUFFER, id);
    }

    OpenglFramebuffer::OpenglFramebuffer(const FramebufferSpec& spec) : Framebuffer(spec)
    {
        glGenFramebuffers(1, &m_bufferId);

        // Split attachmentSet to color attachments and depth attachment
        for (auto& attachment : m_spec.attachmentSet.attachments)
        {
            if (!isDepthFormat(attachment.format))
            {
                m_colorAttachmentSpecs.push_back(attachment);
            }
            else
            {
                m_depthAttachmentSpec = attachment;
            }
        }
    }

    OpenglFramebuffer::~OpenglFramebuffer()
    {
        if (m_bufferId)
        {
            unbind();
            glDeleteFramebuffers(1, &m_bufferId);
            glDeleteTextures(m_colorAttachmentIds.size(), m_colorAttachmentIds.data());
            glDeleteTextures(1, &m_depthAttachmentId);
        }
    }

    OpenglFramebuffer::OpenglFramebuffer(OpenglFramebuffer&& other)
        : m_bufferId(std::move(other.m_bufferId)),
        m_colorAttachmentSpecs(std::move(other.m_colorAttachmentSpecs)),
        m_depthAttachmentSpec(std::move(other.m_depthAttachmentSpec)),
        m_colorAttachmentIds(std::move(other.m_colorAttachmentIds)),
        m_depthAttachmentId(std::move(other.m_depthAttachmentId))
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
        m_colorAttachmentSpecs = std::move(other.m_colorAttachmentSpecs);
        m_depthAttachmentSpec = std::move(other.m_depthAttachmentSpec);
        m_colorAttachmentIds = std::move(other.m_colorAttachmentIds);
        m_depthAttachmentId = std::move(other.m_depthAttachmentId);

        other.m_bufferId = 0;

        return *this;
    }

    std::unique_ptr<Texture2D> OpenglFramebuffer::getColorAttachmentTexture(size_t index /*= 0*/) const
    {
        ASSERT(index < m_colorAttachmentIds.size(), "Invalid index");
        return Texture2D::create(m_colorAttachmentIds[index]);
    }

    void OpenglFramebuffer::invalidate()
    {
        // Bind
        glBindFramebuffer(GL_FRAMEBUFFER, m_bufferId);

        // Cleanup if reusing the framebuffer
        if (m_bufferId)
        {
            glDeleteTextures(m_colorAttachmentIds.size(), m_colorAttachmentIds.data());
            m_colorAttachmentIds.clear();
            glDeleteTextures(1, &m_depthAttachmentId);
            m_depthAttachmentId = 0;
        }

        bool isMultisampled = m_spec.samples > 1;

        // Color attachments
        if (!m_colorAttachmentSpecs.empty())
        {
            m_colorAttachmentIds.resize(m_colorAttachmentSpecs.size());
            m_colorAttachmentIds.reserve(m_colorAttachmentSpecs.size());
            createTextures(isMultisampled, m_colorAttachmentIds.data(), m_colorAttachmentIds.size());

            for (size_t i = 0; i < m_colorAttachmentIds.size(); i++)
            {
                bindTexture(isMultisampled, m_colorAttachmentIds[i]);

                switch (m_colorAttachmentSpecs[i].format)
                {
                case FramebufferTextureFormat::RGBA8:
                    attachColorTexture(m_colorAttachmentIds[i], m_spec.samples, GL_RGBA8,
                                        m_spec.width, m_spec.height, i);
                    break;
                }
            }
        }
        
        // Depth attachment
        if (m_depthAttachmentSpec.format != FramebufferTextureFormat::NONE)
        {
            if (m_depthAttachmentId != 0)
            {
                CM_CORE_LOG_WARN("Overriding an already bound Framebuffer depth attachment");
            }

            switch (m_depthAttachmentSpec.format)
            {
            case FramebufferTextureFormat::DEPTH24_STENCIL8:
                attachDepthRenderBuffer(m_depthAttachmentId, m_spec.samples, GL_DEPTH24_STENCIL8,
                                    GL_DEPTH_STENCIL_ATTACHMENT, m_spec.width, m_spec.height);

                break;
            }
        }

        if (m_colorAttachmentIds.size() > 1)
        {
            ASSERT(m_colorAttachmentIds.size() <= 4, "A maximum of 4 color attachments for Framebuffers are supported");
            GLenum buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
            glDrawBuffers(m_colorAttachmentIds.size(), buffers);
        }
        else if (m_colorAttachmentIds.empty())
        {
            glDrawBuffer(GL_NONE);
        }

        // Check Completeness
        ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

        // Unbind
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenglFramebuffer::renderToScreen()
    {
        // TODO(jcp): Move all this rendering code to its dedicated 2DRenderer?
        if (m_shader == nullptr)
        {
            m_shader = ShaderRegistry::getInstance().getShader("cometFramebufferSwapchain");

            float data[] = {
                // Position     Texture Coords.
                -1.0f, -1.0f,   0.0f, 0.0f,
                 1.0f, -1.0f,   1.0f, 0.0f,
                 1.0f,  1.0f,   1.0f, 1.0f,
                -1.0f,  1.0f,   0.0f, 1.0f
            };

            m_vbo = VertexBuffer::create(GL_STATIC_DRAW);
            auto dataSize = sizeof(data);
            auto dataVectexCount = 4;
            m_vbo->allocate(dataSize);

            m_vao = VertexArray::create();
            auto vboLayout = VertexBufferLayout::create();
            vboLayout->addFloat(2, false, 0); // position
            vboLayout->addFloat(2, false, 1); // texture coordinate
            auto layout = vboLayout.get();
            m_vao->addLayout(*layout, *m_vbo.get(), nullptr);

            m_vbo->mapMemory(GL_WRITE_ONLY);
            m_vbo->loadDataInMappedMemory((const void*)data, dataSize, dataVectexCount);
            m_vbo->unmapMemory();
        }

        if (m_shader && m_spec.swapChainTarget == true)
        {
            glDisable(GL_DEPTH_TEST);

            auto window = Application::getInstance()->getWindow();
            auto width = window->getWidth();
            auto height = window->getHeight();
            glViewport(0, 0, width, height);
            
            m_shader->bind();

            // TODO(jcp): How to handle multiple attachments? Which texture to use/return?
            auto attachmentTexture = getColorAttachmentTexture();
            attachmentTexture->bind();

            m_shader->setUniform("framebufferTexture", 0);
            m_vao->bind();
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
            m_vao->unbind();

            glEnable(GL_DEPTH_TEST);
        }
    }

    void OpenglFramebuffer::clear() const
    {
        glViewport(0, 0, m_spec.width, m_spec.height);
        for (auto& attachmentSpec : m_colorAttachmentSpecs)
        {
            glClearBufferfv(GL_COLOR, 0, (GLfloat*)&attachmentSpec.clearColor);
        }
        
        // TODO(jcp): Add depth clear value in the FramebufferAttachmentSpec (like the clearColor)
        float one{1.0f};
        glClearBufferfv(GL_DEPTH, 0, &one);
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
