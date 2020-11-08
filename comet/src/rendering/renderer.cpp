#include <glad/glad.h>
#include <glm/mat4x4.hpp>
#include <comet/renderer.h>
#include <comet/drawingContext.h>

namespace comet
{
    void Renderer::addMesh(Mesh* mesh)
    {
        auto material = mesh->getMeshInstance().getMaterial();
        auto materialName = material->getName();

        if (m_drawingContexts.find(materialName) == m_drawingContexts.end())
        {
            m_drawingContexts[materialName] = new DrawingContext(material->getShader());
        }

        // Update Buffers size
        if (mesh->getIndexCount())
        {
            m_drawingContexts[materialName]->ibo.increaseSize(mesh->getIndicesSize());
            m_drawingContexts[materialName]->commandBuffer.increaseSize(sizeof(DrawElementsIndirectCommand));
        }
        else
        {
            m_drawingContexts[materialName]->commandBuffer.increaseSize(sizeof(DrawArraysIndirectCommand));
        }

        m_drawingContexts[materialName]->vbo.increaseSize(mesh->getVerticesSize());
        m_drawingContexts[materialName]->instanceBuffer.increaseSize(mesh->getInstancesSize());
        m_drawingContexts[materialName]->meshes.push_back(mesh);
    }

    void Renderer::createAllocateBuffers()
    {
        for (auto [materialName, drawingContext] : m_drawingContexts)
        {
            drawingContext->vbo.allocate();
            drawingContext->instanceBuffer.allocate();
            drawingContext->commandBuffer.allocate();

            VertexBufferLayout vboLayout;
            vboLayout.add<float>(3, false, 0);

            if (drawingContext->ibo.getSize())
            {
                drawingContext->ibo.allocate();
                drawingContext->vao.addLayout(vboLayout, drawingContext->vbo, &drawingContext->ibo);
            }
            else
            {
                drawingContext->vao.addLayout(vboLayout, drawingContext->vbo, nullptr);
            }

            VertexBufferLayout instancedBufferLayout;
            instancedBufferLayout.add<float>(4, false, 1, 1);
            instancedBufferLayout.add<float>(4, false, 2, 1);
            instancedBufferLayout.add<float>(4, false, 3, 1);
            instancedBufferLayout.add<float>(4, false, 4, 1);
            drawingContext->vao.addLayout(instancedBufferLayout, drawingContext->instanceBuffer);
        }
    }

    void Renderer::loadData()
    {
        for (auto [materialName, drawingContext] : m_drawingContexts)
        {
            uint32_t firstIndex{0};
            uint32_t firstVertex{0};
            uint32_t baseVertex{0};
            uint32_t baseInstance{0};

            // Map Buffers memory
            drawingContext->vbo.mapMemory(GL_WRITE_ONLY);
            drawingContext->instanceBuffer.mapMemory(GL_WRITE_ONLY);
            drawingContext->commandBuffer.mapMemory(GL_WRITE_ONLY);
            bool indexBufferExists = (drawingContext->ibo.getSize() > 0);

            if (indexBufferExists)
            {
                drawingContext->ibo.mapMemory(GL_WRITE_ONLY);
            }

            // Load Data from Meshes
            for (auto mesh : drawingContext->meshes)
            {
                uint32_t indexCount = mesh->getIndexCount();
                if (indexCount > 0)
                {
                    firstIndex = drawingContext->ibo.getCurrentCount();
                    auto indices = mesh->getIndices();
                    drawingContext->ibo.loadDataInMappedMemory(indices.data(), indexCount);
                }

                auto vertices = mesh->getVertices();
                auto verticesSize = mesh->getVerticesSize();
                auto vertexCount = mesh->getVertexCount();
                firstVertex = drawingContext->vbo.getCurrentCount();
                drawingContext->vbo.loadDataInMappedMemory((const void*)vertices.data(), verticesSize, vertexCount);

                auto instanceCount =  mesh->getInstanceCount();
                auto baseInstance =  drawingContext->instanceBuffer.getCurrentCount();
                for (auto instance : mesh->getMeshInstances())
                {
                    auto instanceData = instance.getModelTransform();
                    drawingContext->instanceBuffer.loadDataInMappedMemory((const void*)&instanceData, sizeof(instanceData), 1);
                }

                if (indexCount > 1)
                {
                    DrawElementsIndirectCommand cmd{indexCount, instanceCount, firstIndex, baseVertex, baseInstance};
                    drawingContext->commandBuffer.loadDataInMappedMemory((const void*)&cmd, sizeof(cmd), 1);
                }
                else
                {
                    DrawArraysIndirectCommand cmd{vertexCount, instanceCount, firstVertex, baseInstance};
                    drawingContext->commandBuffer.loadDataInMappedMemory((const void*)&cmd, sizeof(cmd), 1);
                }
            }

            drawingContext->ibo.unmapMemory();
            drawingContext->vbo.unmapMemory();
            drawingContext->instanceBuffer.unmapMemory();
            drawingContext->commandBuffer.unmapMemory();
        }
    }

    void Renderer::render(const Camera& camera)
    {
        auto vpMatrix = camera.getViewProjection();
        Shader* shader = nullptr;

        for (auto [materialName, drawingContext] : m_drawingContexts)
        {
            if (shader != drawingContext->shader)
            {
                shader = drawingContext->shader;
                shader->bind();
            }
            
            shader->setUniform(shader->getViewProjectionMatrixName(), vpMatrix);
            shader->setUniform("u_flatColor", glm::vec4(1.0f, 0.4f, 0.0f, 1.0f));

            drawingContext->vao.bind();
            drawingContext->commandBuffer.bind();

            for (auto mesh : drawingContext->meshes)
            {
                uint32_t indexCount = mesh->getIndexCount();
                if (indexCount > 1)
                {
                    glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, nullptr, 1, 0);
                }
                else
                {
                    glMultiDrawArraysIndirect(GL_TRIANGLES, nullptr, 1, 0);
                }
            }
        }
    }

} // namespace comet