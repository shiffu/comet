#include <glad/glad.h>
#include <comet/vertexArray.h>
#include <comet/log.h>

namespace comet
{

    VertexArray::VertexArray()
    {
        glGenVertexArrays(1, &m_vao);
    }

    VertexArray::~VertexArray()
    {
        if (m_vao)
        {
            unbind();
            glDeleteVertexArrays(1, &m_vao);
        }
    }

    VertexArray::VertexArray(VertexArray&& other)
        : m_vao(std::move(other.m_vao))
    {
        other.m_vao = 0;
    }

    VertexArray& VertexArray::operator=(VertexArray&& other) noexcept
    {
        if (&other == this)
        {
            return *this;
        }
        if (m_vao)
        {
            unbind();
            glDeleteVertexArrays(1, &m_vao);
        }

        m_vao = std::move(other.m_vao);

        other.m_vao = 0;

        return *this;
    }

    void VertexArray::bind() const { glBindVertexArray(m_vao); }
    void VertexArray::unbind() const { glBindVertexArray(0); }

    void VertexArray::addLayout(const VertexBufferLayout& vbl, const VertexBuffer& vbo, const IndexBuffer* ibo /*= nullptr*/)
    {
            const auto& bufferAttributes = vbl.getAttributes();
            unsigned int offset = 0;
            bind();
            vbo.bind();
            if (ibo)
            {
                ibo->bind();
            }

            // Loop over the vertex attributes
            for (unsigned int i = 0; i < bufferAttributes.size(); ++i) {
                const auto& attr = bufferAttributes[i];
                
                glEnableVertexAttribArray(attr.loc);
                CM_CORE_LOG_DEBUG("enabled vertex attrib pointer: loc = {}, count = {}, type = {}, normalized = {}, stride = {}", attr.loc, attr.count, attr.type, attr.normalized, vbl.getStride());
                
                switch(attr.type)
                {
                    case GL_BYTE:
                    case GL_UNSIGNED_BYTE:
                    case GL_SHORT:
                    case GL_UNSIGNED_SHORT:
                    case GL_INT:
                    case GL_UNSIGNED_INT:
                        glVertexAttribIPointer(attr.loc, attr.count, attr.type, vbl.getStride(),
                                                reinterpret_cast<void*>(offset));
                        break;

                    default:
                        glVertexAttribPointer(attr.loc, attr.count, attr.type, attr.normalized, vbl.getStride(),
                                                reinterpret_cast<void*>(offset));
                        break;
                }

                if (attr.divisor)
                {
                    glVertexAttribDivisor(attr.loc, attr.divisor);
                }
                offset += attr.count * attr.size;
            }
            unbind();
    }
    
} // namespace comet
