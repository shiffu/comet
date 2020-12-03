#include <comet/phongMaterial.h>
#include <comet/textureRegistry.h>
#include <core/resourceManager.h>
#include <sstream>

namespace comet
{
    const char* PhongMaterial::MATERIAL_NAME = "cometPhongMaterial";
    const char* PhongMaterial::SHADER_NAME = "cometPhong";
    const char* PhongMaterial::MATERIAL_ALBEDO_TEXTURE_NAME = "cometPhongMaterial-AlbedoTextureArray";


    std::vector<PhongMaterial*>& PhongMaterial::getMaterialInstances()
    {
        static std::vector<PhongMaterial*> s_materialInstances{};
        return s_materialInstances;
    }

    PhongMaterial::PhongMaterial()
        : Material(MATERIAL_NAME, SHADER_NAME),
            m_diffuse({1.0}), m_specular({0.0f}), m_shininess(0.1f)
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

    Texture2DArray* PhongMaterial::getAlbedoTextureArray()
    {
        static Texture2DArray* albedoTexture = TextureRegistry::getInstance().getTexture2DArray(PhongMaterial::MATERIAL_ALBEDO_TEXTURE_NAME);
        return albedoTexture;
    }

    void PhongMaterial::setAlbedoTexture(const char* filename)
    {
        auto textureArray = getAlbedoTextureArray();
        m_albedoTextureIndex = textureArray->addTexture2D(filename);
    }

    void PhongMaterial::loadUniforms()
    {
        getShader();

        std::vector<glm::vec3> diffuseColors{};
        std::vector<glm::vec3> specularColors{};
        std::vector<float> shininessFactors{};

        auto materialInstances = getMaterialInstances();
        auto materialInstanceCount = materialInstances.size();
        diffuseColors.reserve(materialInstanceCount);
        specularColors.reserve(materialInstanceCount);
        shininessFactors.reserve(materialInstanceCount);

        std::stringstream ss;
        for (int i = 0; i <  materialInstanceCount; ++i)
        {
            ss << "material_instances[" << i << "].diffuse";
            m_shader->setUniform(ss.str(), materialInstances[i]->getDiffuse()); ss.str(""); ss.clear();

            ss << "material_instances[" << i << "].specular";
            m_shader->setUniform(ss.str(), materialInstances[i]->getSpecular()); ss.str(""); ss.clear();

            ss << "material_instances[" << i << "].shininess";
            m_shader->setUniform(ss.str(), materialInstances[i]->getShininess()); ss.str(""); ss.clear();

            ss << "material_instances[" << i << "].albedo_texture_idx";
            m_shader->setUniform(ss.str(), materialInstances[i]->getAlbedoTextureIndex()); ss.str(""); ss.clear();
        }

        auto textureArray = getAlbedoTextureArray();
        if (textureArray->getTextureCount())
        {
            if (textureArray->needToLoadTextureArray())
            {
                textureArray->load();
            }
            else
            {
                textureArray->bind(1);
            }
            
            m_shader->setUniform("albedo_textures", 1);
        }

        TextureRegistry::getInstance().getGeneratedWhiteTexture2D()->bind();
        m_shader->setUniform("white_1x1_texture", 0);
    }

} // namespace comet