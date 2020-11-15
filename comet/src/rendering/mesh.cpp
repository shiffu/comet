#include <comet/mesh.h>

namespace comet
{
    
    Mesh::Mesh(Vertex* vertices, uint32_t vertexCount)
    {
        setVertices(vertices, vertexCount);
    }
    
    Mesh::Mesh(Vertex* vertices, uint32_t vertexCount, const uint32_t* indices, uint32_t indexCount)
    {
        setData(vertices, vertexCount, indices, indexCount);
    }

    MeshInstance& Mesh::createMeshInstance()
    {
        m_meshInstances.emplace_back(this);
        
        return m_meshInstances.back();
    }

    void Mesh::setData(Vertex* vertices, uint32_t vertexCount, const uint32_t* indices, uint32_t indexCount)
    {
        setVertices(vertices, vertexCount);
        setIndices(indices, indexCount);
    }

    void Mesh::setVertices(Vertex* data, uint32_t vertexCount)
    {
        m_vertices.clear();
        m_vertices.reserve(vertexCount);
        for(uint32_t i = 0; i < vertexCount; ++i)
        {
            m_vertices.push_back(data[i]);
        }
        m_vertexCount = vertexCount;
    }

    void Mesh::setIndices(const uint32_t* indices, uint32_t indexCount)
    {
        m_indices.clear();
        m_indices.reserve(indexCount);
        for(uint32_t i = 0; i < indexCount; ++i)
        {
            m_indices.push_back(indices[i]);
        }
        m_indexCount = indexCount;
    }

    void MeshInstance::setMaterial(Material* material)
    {
        m_material = material;
        setMaterialInstanceId(m_material->getInstanceId());
    }

    void MeshInstance::setModelTransform(const glm::mat4& modelTransform)
    {
        m_instanceData.modelTransform = modelTransform;
    }
    
} // namespace comet