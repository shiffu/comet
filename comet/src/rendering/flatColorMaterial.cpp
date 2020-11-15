#include <comet/flatColorMaterial.h>

#include <comet/shaderRegistry.h>
#include <iostream>

namespace comet
{
    const char* FlatColorMaterial::MATERIAL_NAME = "cometFlatColorMaterial";
    const char* FlatColorMaterial::SHADER_NAME = "cometFlatColorShader";

    std::vector<FlatColorMaterial>& FlatColorMaterial::getMaterialInstances()
    {
        static std::vector<FlatColorMaterial> s_materialInstances{};
        return s_materialInstances;
    }

    FlatColorMaterial::FlatColorMaterial()
        : Material(MATERIAL_NAME)
    {
        // TODO: Use a Resource Manager
        m_instanceID = getMaterialInstances().size();
        getMaterialInstances().push_back(*this);
    }

    FlatColorMaterial::FlatColorMaterial(const glm::vec4& color)
        : Material(MATERIAL_NAME), m_color(color)
    {
        m_instanceID = getMaterialInstances().size();
        getMaterialInstances().push_back(*this);
    }

    Shader* FlatColorMaterial::getShader()
    {
        if (m_shader == nullptr)
        {
            m_shader = ShaderRegistry::getInstance().getShader(SHADER_NAME);
        }
        return m_shader;
    }

    void FlatColorMaterial::loadUniforms()
    {
        getShader();
        m_shader->bind();
        std::vector<glm::vec4> data{};
        auto materialInstanceCount = getMaterialInstances().size();
        data.reserve(materialInstanceCount);
        for (auto& materialInstance : getInstances())
        {
            data.push_back(materialInstance.getColor());
        }
        m_shader->setUniform("u_flatColor", materialInstanceCount, (glm::vec4*)data.data());
    }

} // namespace comet