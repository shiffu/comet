#pragma once

#include <comet/material.h>
#include <vector>
#include <glm/vec4.hpp>

namespace comet
{
    class VertexBufferLayout;

    class FlatColorMaterial : public Material
    {
    public:
        FlatColorMaterial();
        FlatColorMaterial(const glm::vec4& color);
        ~FlatColorMaterial() = default;

        uint32_t getTypeHash() const override;

        void setColor(const glm::vec4& color) { m_color = color; }
        const glm::vec4& getColor() const { return m_color; }

        virtual void loadUniforms() override;
    
        // Vertex and Instance Buffers Layout
        virtual void updateVboDataLayout(VertexBufferLayout& layout) const override;
        virtual void updateInstanceDataLayout(VertexBufferLayout& layout) const override;

    private:
        static const char* MATERIAL_NAME;
        static const char* SHADER_NAME;

    private:
        glm::vec4 m_color;
    };
    
} // namespace comet