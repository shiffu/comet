#pragma once

#include <comet/material.h>
#include <vector>
#include <glm/vec4.hpp>

namespace comet
{

    class FlatColorMaterial : public Material
    {
    public:
        FlatColorMaterial();
        FlatColorMaterial(const glm::vec4& color);
        ~FlatColorMaterial() {}

        void setColor(const glm::vec4& color) { m_color = color; }
        const glm::vec4& getColor() const { return m_color; }

        std::vector<FlatColorMaterial>& getInstances() { return getMaterialInstances(); }

        Shader* getShader() override;
        void loadUniforms() override;
    
    private:
        std::vector<FlatColorMaterial>& getMaterialInstances();

    private:
        static const char* MATERIAL_NAME;
        static const char* SHADER_NAME;

    private:
        glm::vec4 m_color;
    };
    
} // namespace comet