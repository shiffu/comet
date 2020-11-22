#include <comet/phongMaterial.h>

namespace comet
{
    const char* PhongMaterial::MATERIAL_NAME = "cometPhongMaterial";
    const char* PhongMaterial::SHADER_NAME = "cometPhong";

    std::vector<PhongMaterial*>& PhongMaterial::getMaterialInstances()
    {
        static std::vector<PhongMaterial*> s_materialInstances{};
        return s_materialInstances;
    }

    PhongMaterial::PhongMaterial()
        : Material(MATERIAL_NAME, SHADER_NAME),
            m_diffuse({0.8}), m_specular({0.0f}), m_shininess(0.1f)
    {       
        init(); 
    }

    PhongMaterial::PhongMaterial(const glm::vec3& diffuse)
        : Material(MATERIAL_NAME, SHADER_NAME),
        m_diffuse(diffuse), m_specular({0.0f}), m_shininess(0.1f)
    {
        init(); 
    }

    PhongMaterial::PhongMaterial(const glm::vec3& diffuse, const glm::vec3& specular)
        : Material(MATERIAL_NAME, SHADER_NAME),
        m_diffuse(diffuse), m_specular(specular), m_shininess(0.1f)
    {
        init(); 
    }

    PhongMaterial::PhongMaterial(const glm::vec3& diffuse, const glm::vec3& specular, float shininess)
        : Material(MATERIAL_NAME, SHADER_NAME),
        m_diffuse(diffuse), m_specular(specular), m_shininess(shininess)
    {
        init(); 
    }

    void PhongMaterial::init()
    {
        m_instanceID = getMaterialInstances().size();
        getMaterialInstances().push_back(this);
    }

    void PhongMaterial::loadUniforms()
    {
        getShader();
        m_shader->bind();
        std::vector<glm::vec3> diffuseColors{};
        std::vector<glm::vec3> specularColors{};
        std::vector<float> shininessFactors{};

        auto materialInstances = getMaterialInstances();
        auto materialInstanceCount = materialInstances.size();
        diffuseColors.reserve(materialInstanceCount);
        specularColors.reserve(materialInstanceCount);
        shininessFactors.reserve(materialInstanceCount);

        for (auto materialInstance : materialInstances)
        {
            diffuseColors.push_back(materialInstance->getDiffuse());
            specularColors.push_back(materialInstance->getSpecular());
            shininessFactors.push_back(materialInstance->getShininess());
        }

        m_shader->setUniform("material_diffuse", materialInstanceCount, (glm::vec3*)diffuseColors.data());
        m_shader->setUniform("material_specular", materialInstanceCount, (glm::vec3*)specularColors.data());
        m_shader->setUniform("material_shininess", materialInstanceCount, (float*)shininessFactors.data());
    }

} // namespace comet