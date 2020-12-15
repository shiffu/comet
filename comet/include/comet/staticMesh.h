#pragma once

#include <vector>
#include <memory>
#include <comet/vertex.h>
#include <comet/vertexBufferLayout.h>

namespace comet
{

    class StaticMesh
    {
    public:
        StaticMesh(const char* filename);
        StaticMesh(Vertex* vertices, uint32_t vertexCount);
        StaticMesh(Vertex* vertices, uint32_t vertexCount, const uint32_t* indices, uint32_t indexCount);

        void setData(Vertex* vertices, uint32_t vertexCount, const uint32_t* indices, uint32_t indexCount);

        void setVertices(Vertex* vertices, uint32_t vertexCount);
        const std::vector<Vertex>& getVertices() const { return m_vertices; }
        std::vector<Vertex>& getVertices() { return m_vertices; }
        uint32_t getVertexCount() const { return m_vertexCount; }
        uint32_t getVerticesSize() const { return m_vertexCount * sizeof(Vertex); }

        bool hasIndices() const { return m_indexCount != 0; }
        void setIndices(const uint32_t* indices, uint32_t indexCount);
        const std::vector<uint32_t>& getIndices() const { return m_indices; }
        std::vector<uint32_t>& getIndices() { return m_indices; }
        uint32_t getIndexCount() const { return m_indexCount; }
        uint32_t getIndicesSize() const { return m_indexCount * sizeof(uint32_t); }

    private:
        uint32_t m_indexCount{0};
        uint32_t m_vertexCount{0};
        std::vector<uint32_t> m_indices{};
        std::vector<Vertex> m_vertices{};
    };

} // namespace comet
