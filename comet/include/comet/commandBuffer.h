#pragma once
#include <cstdint>
#include <comet/buffer.h>

namespace comet
{
    struct DrawArraysIndirectCommand
    {
        DrawArraysIndirectCommand(
            uint32_t _vertexCount,
            uint32_t _instanceCount,
            uint32_t _firstVertex,
            uint32_t _baseInstance) : vertexCount(_vertexCount), instanceCount(_instanceCount),
                                    firstVertex(_firstVertex), baseInstance(_baseInstance) {}

        GLuint vertexCount;
        GLuint instanceCount;
        GLuint firstVertex;
        GLuint baseInstance;
    };

    struct DrawElementsIndirectCommand
    {
        DrawElementsIndirectCommand(
            uint32_t _indexCount,
            uint32_t _instanceCount,
            uint32_t _firstIndex,
            uint32_t _baseVertex,
            uint32_t _baseInstance) : indexCount(_indexCount), instanceCount(_instanceCount),
                                    firstIndex(_firstIndex), baseVertex(_baseVertex), baseInstance(_baseInstance) {}

        GLuint indexCount;
        GLuint instanceCount;
        GLuint firstIndex;
        GLuint baseVertex;
        GLuint baseInstance;
    };

    class CommandBuffer : public Buffer
    {
    public:
        CommandBuffer(uint32_t usage);
        ~CommandBuffer() {};

        CommandBuffer& operator=(CommandBuffer&& other) noexcept;
    };
}