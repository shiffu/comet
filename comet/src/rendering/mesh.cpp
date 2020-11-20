#include <comet/mesh.h>

#include <glm/gtc/matrix_transform.hpp>
#include <core/resourceManager.h>
#include <core/objLoader.h>

namespace comet
{
    
    Mesh::Mesh(const char* filename)
    {
        auto meshResourcePath = ResourceManager::getInstance().getResourcePath(ResourceType::MESH, filename);
        ObjLoader loader;
        loader.loadFile(meshResourcePath, m_vertices, m_indices);
        m_vertexCount = m_vertices.size();
        m_indexCount = m_indices.size();
    }

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

    void Mesh::updateVboDataLayout(VertexBufferLayout& layout) const
    {
        //TODO: We are introducing a dependency to Shader's attributes here. To think about it.
        layout.add<float>(3, false, 0);
        layout.add<float>(3, false, 1);
        layout.add<float>(2, false, 2);
    }

    void Mesh::updateInstanceDataLayout(VertexBufferLayout& layout) const
    {
        layout.add<float>(4, false, 10, 1);
        layout.add<float>(4, false, 11, 1);
        layout.add<float>(4, false, 12, 1);
        layout.add<float>(4, false, 13, 1);
        layout.add<unsigned int>(1, false, 14, 1);
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

    void MeshInstance::move(const glm::vec3& translation)
    {
        m_instanceData.modelTransform = glm::translate(m_instanceData.modelTransform, translation);
    }

    void MeshInstance::rotate(float angle, const glm::vec3& axis)
    {
        m_instanceData.modelTransform = glm::rotate(m_instanceData.modelTransform, angle, axis);
    }

    void MeshInstance::scale(const glm::vec3& values)
    {
        m_instanceData.modelTransform = glm::scale(m_instanceData.modelTransform, values);
    }

    void MeshInstance::scale(float value)
    {
        m_instanceData.modelTransform = glm::scale(m_instanceData.modelTransform, glm::vec3(value, value, value));
    }
    
} // namespace comet