#include <glm/gtc/matrix_transform.hpp>
#include <comet/resourceManager.h>
#include <core/objLoader.h>

namespace comet
{
    
    StaticMesh::StaticMesh(const char* filename) : m_name(filename)
    {
        auto meshResourcePath = ResourceManager::getInstance().getResourcePath(ResourceType::MESH, filename);
        ObjLoader loader;
        loader.loadFile(meshResourcePath, m_vertices, m_indices);
        m_vertexCount = m_vertices.size();
        m_indexCount = m_indices.size();
    }

    StaticMesh::StaticMesh(const char* name, Vertex* vertices, uint32_t vertexCount)
         : m_name(name)
    {
        setVertices(vertices, vertexCount);
    }
    
    StaticMesh::StaticMesh(const char* name, Vertex* vertices, uint32_t vertexCount, const uint32_t* indices, uint32_t indexCount)
         : m_name(name)
    {
        setData(vertices, vertexCount, indices, indexCount);
    }

    void StaticMesh::setData(Vertex* vertices, uint32_t vertexCount, const uint32_t* indices, uint32_t indexCount)
    {
        setVertices(vertices, vertexCount);
        setIndices(indices, indexCount);
    }

    void StaticMesh::setVertices(Vertex* data, uint32_t vertexCount)
    {
        m_vertices.clear();
        m_vertices.reserve(vertexCount);
        for(uint32_t i = 0; i < vertexCount; ++i)
        {
            m_vertices.push_back(data[i]);
        }
        m_vertexCount = vertexCount;
    }

    void StaticMesh::setIndices(const uint32_t* indices, uint32_t indexCount)
    {
        m_indices.clear();
        m_indices.reserve(indexCount);
        for(uint32_t i = 0; i < indexCount; ++i)
        {
            m_indices.push_back(indices[i]);
        }
        m_indexCount = indexCount;
    }

} // namespace comet