#include <glad/glad.h>
#include <utility>
#include <comet/vertexBuffer.h>

namespace comet
{
    VertexBuffer::VertexBuffer(uint32_t usage)
        : Buffer(GL_ARRAY_BUFFER, usage)
    {
    }

    VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept
    {
        Buffer::operator=(std::move(other));
        return *this;
    }    
}