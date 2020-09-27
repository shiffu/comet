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

    void VertexArray::bind() const { glBindVertexArray(m_vao); }
    void VertexArray::unbind() const { glBindVertexArray(0); }

    void VertexArray::addLayout(const VertexBuffer& vbo, const VertexBufferLayout& vbl)
    {
            const auto& bufferAttributes = vbl.getAttributes();
            unsigned int offset = 0;
            bind();
            vbo.bind();
            for (unsigned int i = 0; i < bufferAttributes.size(); ++i) {
                const auto& attr = bufferAttributes[i];
                CM_CORE_LOG_DEBUG("enable vertex attrib {}", i);
                glEnableVertexAttribArray(i);
                CM_CORE_LOG_DEBUG("vertex attrib pointer: count = {}, type = {}, normalized = {}, stride = {}", attr.count, attr.type, attr.normalized, vbl.getStride());
                glVertexAttribPointer(i, attr.count, attr.type, attr.normalized, vbl.getStride(),
                                    reinterpret_cast<void*>(offset));
                offset += attr.count * attr.size;
            }
            unbind();
    }
    
} // namespace comet
