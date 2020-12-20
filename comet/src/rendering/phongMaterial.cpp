#include <comet/phongMaterial.h>
#include <comet/textureRegistry.h>
#include <comet/resourceManager.h>
#include <comet/materialRegistry.h>
#include <comet/utils.h>
#include <sstream>

namespace comet
{
    const char* PhongMaterial::MATERIAL_NAME = "cometPhongMaterial";
    const char* PhongMaterial::SHADER_NAME = "cometPhong";
    const char* PhongMaterial::MATERIAL_ALBEDO_TEXTURE_NAME = "cometPhongMaterial-AlbedoTextureArray";

    constexpr uint32_t getTypeHashImpl()
    {
        constexpr uint32_t typeHash = utils::hashStr("PhongMaterial");
        return typeHash;
    }

    PhongMaterial::PhongMaterial()
        : Material(MATERIAL_NAME, SHADER_NAME),
            m_diffuse({1.0}), m_specular({0.0f}), m_shininess(0.1f)
    {       
    }

    PhongMaterial::PhongMaterial(const glm::vec3& diffuse)
        : Material(MATERIAL_NAME, SHADER_NAME),
        m_diffuse(diffuse), m_specular({0.0f}), m_shininess(0.1f)
    {
    }

    PhongMaterial::PhongMaterial(const glm::vec3& diffuse, const glm::vec3& specular)
        : Material(MATERIAL_NAME, SHADER_NAME),
        m_diffuse(diffuse), m_specular(specular), m_shininess(0.1f)
    {
    }

    PhongMaterial::PhongMaterial(const glm::vec3& diffuse, const glm::vec3& specular, float shininess)
        : Material(MATERIAL_NAME, SHADER_NAME),
        m_diffuse(diffuse), m_specular(specular), m_shininess(shininess)
    {
    }

    uint32_t PhongMaterial::getTypeHash() const
    {
        return getTypeHashImpl();
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

    // Vertex and Instance Buffers Layout
    void PhongMaterial::updateVboDataLayout(VertexBufferLayout& layout) const
    {
        layout.addFloat(3, false, 0); // position
        layout.addFloat(3, false, 1); // normal
        layout.addFloat(2, false, 2); // texture coordinate
    }

    void PhongMaterial::updateInstanceDataLayout(VertexBufferLayout& layout) const
    {
        layout.addFloat(4, false, 10, 1); //
        layout.addFloat(4, false, 11, 1); //
        layout.addFloat(4, false, 12, 1); // Instance Model to Word Matrix transform
        layout.addFloat(4, false, 13, 1); //
        layout.addUInt(1, false, 14, 1);  // Material ID (or index)
    }

    void PhongMaterial::loadUniforms()
    {
        getShader();

        std::vector<glm::vec3> diffuseColors{};
        std::vector<glm::vec3> specularColors{};
        std::vector<float> shininessFactors{};

        auto materialInstancesPtr = MaterialRegistry::getInstance().getMaterialInstances(getTypeHashImpl());
        if (materialInstancesPtr)
        {
            auto& materialInstances = *materialInstancesPtr;
            auto materialInstanceCount = materialInstances.size();
            diffuseColors.reserve(materialInstanceCount);
            specularColors.reserve(materialInstanceCount);
            shininessFactors.reserve(materialInstanceCount);

            std::stringstream ss;
            for (int i = 0; i <  materialInstanceCount; ++i)
            {
                auto material = static_cast<PhongMaterial*>(materialInstances[i].get());

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