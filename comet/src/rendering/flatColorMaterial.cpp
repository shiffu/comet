#include <comet/flatColorMaterial.h>

namespace comet
{
    const char* FlatColorMaterial::MATERIAL_NAME = "cometFlatColorMaterial";
    const char* FlatColorMaterial::SHADER_NAME = "cometFlatColor";

    std::vector<FlatColorMaterial*>& FlatColorMaterial::getMaterialInstances()
    {
        static std::vector<FlatColorMaterial*> s_materialInstances{};
        return s_materialInstances;
    }

    FlatColorMaterial::FlatColorMaterial()
        : Material(MATERIAL_NAME, SHADER_NAME)
    {
        // TODO: Use a Resource Manager
        m_instanceID = getMaterialInstances().size();
        getMaterialInstances().push_back(this);
    }

    FlatColorMaterial::FlatColorMaterial(const glm::vec4& color)
        : Material(MATERIAL_NAME, SHADER_NAME), m_color(color)
    {
        m_instanceID = getMaterialInstances().size();
        getMaterialInstances().push_back(this);
    }

    void FlatColorMaterial::loadUniforms()
    {
        getShader();
        m_shader->bind();

        auto materialInstances = getMaterialInstances();
        auto materialInstanceCount = materialInstances.size();
        std::vector<glm::vec4> colors{};
        colors.reserve(materialInstanceCount);

        for (auto& materialInstance : materialInstances)
        {
            colors.push_back(materialInstance->getColor());
        }
        m_shader->setUniform("u_flatColor", materialInstanceCount, (glm::vec4*)colors.data());
    }

} // namespace comet