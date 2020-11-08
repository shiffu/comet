#include <glad/glad.h>
#include <utility>
#include <type_traits>
#include <comet/commandBuffer.h>

namespace comet
{
    CommandBuffer::CommandBuffer(uint32_t usage)
        : Buffer(GL_DRAW_INDIRECT_BUFFER, usage)
    {
    }

    CommandBuffer& CommandBuffer::operator=(CommandBuffer&& other) noexcept
    {
        Buffer::operator=(std::move(other));
        return *this;
    }
}