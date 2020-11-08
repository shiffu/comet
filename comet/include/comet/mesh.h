#pragma once

#include <vector>
#include <glm/mat4x4.hpp>
#include <comet/vertex.h>
#include <comet/material.h>

namespace comet
{

    class MeshInstance
    {
    public:
        void setMaterial(Material* material);
        Material* getMaterial() { return m_material; }
        
        void setModelTransform(const glm::mat4& modelTransform);
        glm::mat4& getModelTransform() { return m_modelTransform; }

    private:
        Material* m_material = nullptr;
        glm::mat4 m_modelTransform{1.0f};
    };


    class Mesh
    {
    public:
        Mesh(Vertex* vertices, uint32_t vertexCount);
        Mesh(Vertex* vertices, uint32_t vertexCount, const uint32_t* indices, uint32_t indexCount);

        void setData(Vertex* vertices, uint32_t vertexCount, const uint32_t* indices, uint32_t indexCount);

        void setVertices(Vertex* vertices, uint32_t vertexCount);
        const std::vector<Vertex>& getVertices() const { return m_vertices; }
        uint32_t getVertexCount() const { return m_vertexCount; }
        uint32_t getVerticesSize() const { return m_vertexCount * sizeof(Vertex); }

        void setIndices(const uint32_t* indices, uint32_t indexCount);
        const std::vector<uint32_t>& getIndices() const { return m_indices; }
        uint32_t getIndexCount() const { return m_indexCount; }
        uint32_t getIndicesSize() const { return m_indexCount * sizeof(uint32_t); }

        MeshInstance& createMeshInstance() { m_meshInstances.push_back(MeshInstance()); return m_meshInstances.back(); };
        MeshInstance& getMeshInstance() { return m_meshInstances[0]; };
        std::vector<MeshInstance>& getMeshInstances() { return m_meshInstances; };
        uint32_t getInstanceCount() const { return m_meshInstances.size(); }
        size_t getInstancesSize() const { return m_meshInstances.size() * sizeof(glm::mat4); }

    private:
        uint32_t m_indexCount{0};
        uint32_t m_vertexCount{0};
        std::vector<uint32_t> m_indices{};
        std::vector<Vertex> m_vertices{};
        std::vector<MeshInstance> m_meshInstances{};
    };

} // namespace comet
