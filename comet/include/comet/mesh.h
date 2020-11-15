#pragma once

#include <vector>
#include <comet/vertex.h>
#include <glm/mat4x4.hpp>
#include <comet/material.h>

namespace comet
{

    class Mesh;

    struct MeshInstanceData
    {
        glm::mat4 modelTransform{1.0f};
        uint32_t materialInstanceId{0};
    };

    class MeshInstance
    {
    public:
        MeshInstance(Mesh* mesh) : m_mesh(mesh) {};

        // TODO: for the moment there is no check that all instances have the same material, but they must have.
        // Need to find a way to add this material constraint
        void setMaterial(Material* material);
        Material* getMaterial() { return m_material; }
        
        void setModelTransform(const glm::mat4& modelTransform);
        glm::mat4& getModelTransform() { return m_instanceData.modelTransform; }

        void setMaterialInstanceId(uint32_t materialInstanceId) { m_instanceData.materialInstanceId = materialInstanceId; }

        MeshInstanceData& getInstanceData() { return m_instanceData; }

    private:
        Material* m_material{nullptr};
        Mesh* m_mesh{nullptr};
        MeshInstanceData m_instanceData{};
    };


    class Mesh
    {
        friend class MeshInstance;

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

        void setMeshMaterial(Material* material) { m_meshMaterial = material; }
        Material* getMeshMaterial() { return m_meshMaterial; }

        MeshInstance& createMeshInstance();
        std::vector<MeshInstance>& getMeshInstances() { return m_meshInstances; };
        uint32_t getInstanceCount() const { return m_meshInstances.size(); }
        size_t getInstanceDataSize() const { return sizeof(MeshInstanceData); }

    private:
        uint32_t m_indexCount{0};
        uint32_t m_vertexCount{0};
        std::vector<uint32_t> m_indices{};
        std::vector<Vertex> m_vertices{};
        std::vector<MeshInstance> m_meshInstances{};
        Material* m_meshMaterial{nullptr};
    };

} // namespace comet
