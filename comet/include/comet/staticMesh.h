#pragma once

#include <comet/vertex.h>
#include <rendering/vertexBufferLayout.h>

#include <vector>
#include <memory>
#include <string>

namespace comet
{

    class StaticMesh
    {
    public:
        StaticMesh(const char* filename);
        StaticMesh(const char* name, Vertex* vertices, uint32_t vertexCount);
        StaticMesh(const char* name, Vertex* vertices, uint32_t vertexCount, const uint32_t* indices, uint32_t indexCount);

        const std::string& getName() const { return m_name; }
        void setData(Vertex* vertices, uint32_t vertexCount, const uint32_t* indices, uint32_t indexCount);

        void setVertices(Vertex* vertices, uint32_t vertexCount);
        const std::vector<Vertex>& getVertices() const { return m_vertices; }
        std::vector<Vertex>& getVertices() { return m_vertices; }
        uint32_t getVertexCount() const { return m_vertexCount; }
        size_t getVerticesSize() const { return m_vertexCount * sizeof(Vertex); }

        bool hasIndices() const { return m_indexCount != 0; }
        void setIndices(const uint32_t* indices, uint32_t indexCount);
        const std::vector<uint32_t>& getIndices() const { return m_indices; }
        std::vector<uint32_t>& getIndices() { return m_indices; }
        uint32_t getIndexCount() const { return m_indexCount; }
        size_t getIndicesSize() const { return m_indexCount * sizeof(uint32_t); }

    private:
        std::string m_name;
        uint32_t m_indexCount{0};
        uint32_t m_vertexCount{0};
        std::vector<uint32_t> m_indices{};
        std::vector<Vertex> m_vertices{};
    };

} // namespace comet
