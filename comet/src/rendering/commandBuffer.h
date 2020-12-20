#pragma once

#include <rendering/buffer.h>

#include <memory>

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

        uint32_t vertexCount;
        uint32_t instanceCount;
        uint32_t firstVertex;
        uint32_t baseInstance;
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

        uint32_t indexCount;
        uint32_t instanceCount;
        uint32_t firstIndex;
        uint32_t baseVertex;
        uint32_t baseInstance;
    };

    class CommandBuffer : public Buffer
    {
    public:
        virtual ~CommandBuffer() {};

        virtual void loadDataInMappedMemory(const void* data, size_t size, uint32_t count, uint32_t offset = 0) = 0;

        static std::unique_ptr<CommandBuffer> create(uint32_t usage);
    };
}