#include <comet/material.h>
#include <comet/shaderRegistry.h>

#include <comet/textureRegistry.h>
#include <comet/resourceManager.h>
#include <comet/materialRegistry.h>
#include <comet/utils.h>

#include <sstream>

namespace comet
{
    const char* Material::MATERIAL_ALBEDO_TEXTURE_NAME = "cometMaterial-AlbedoTextureArray";

    Shader* Material::getShader()
    {
        if (m_shader == nullptr)
        {
            m_shader = ShaderRegistry::getInstance().getShader(m_shaderName.c_str());
        }
        return m_shader;
    }

    Texture2DArray* Material::getAlbedoTextureArray()
    {
        static Texture2DArray* albedoTexture = TextureRegistry::getInstance().getTexture2DArray(Material::MATERIAL_ALBEDO_TEXTURE_NAME);
        return albedoTexture;
    }

    void Material::setAlbedoTexture(const std::string& filename)
    {
        if (!filename.empty())
        {
            m_albedoTextureFilename = filename;
            auto textureArray = getAlbedoTextureArray();
            m_albedoTextureIndex = textureArray->addTexture2D(filename.c_str());
        }
    }

    void Material::loadUniforms()
    {
        getShader();

        std::vector<glm::vec3> diffuseColors{};
        std::vector<glm::vec3> specularColors{};
        std::vector<float> shininessFactors{};

        auto& materialInstances = MaterialRegistry::getInstance().getMaterialInstances();
        if (!materialInstances.empty())
        {
            auto materialInstanceCount = materialInstances.size();
            diffuseColors.reserve(materialInstanceCount);
            specularColors.reserve(materialInstanceCount);
            shininessFactors.reserve(materialInstanceCount);

            std::stringstream ss;
            for (int i = 0; i <  materialInstanceCount; ++i)
            {
                auto material = materialInstances[i].get();

                ss << "material_instances[" << i << "].diffuse";
                m_shader->setUniform(ss.str(), material->getDiffuse()); ss.str(""); ss.clear();

                ss << "material_instances[" << i << "].specular";
                m_shader->setUniform(ss.str(), material->getSpecular()); ss.str(""); ss.clear();

                ss << "material_instances[" << i << "].shininess";
                m_shader->setUniform(ss.str(), material->getShininess()); ss.str(""); ss.clear();

                ss << "material_instances[" << i << "].albedo_texture_idx";
                m_shader->setUniform(ss.str(), material->getAlbedoTextureIndex()); ss.str(""); ss.clear();
            }
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

        TextureRegistry::getInstance().getWhiteTexture2D()->bind();
        m_shader->setUniform("white_1x1_texture", 0);
    }

} // namespace comet
