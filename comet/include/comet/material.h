#pragma once

#include <glm/vec3.hpp>

#include <memory>

namespace comet
{

    class Shader;
    class Texture2DArray;
    class MaterialRegistry;
    class VertexBufferLayout;
    
    class Material
    {
        friend class MaterialRegistry;

    public:
        Material(const char* name, const char* shaderName = "cometPhong")
            : m_name(name), m_shaderName(shaderName) {}

        std::string& getName() { return m_name; }
        const std::string& getName() const { return m_name; }
        const std::string& getShaderName() { return m_shaderName; }

        uint32_t getInstanceId() const { return m_instanceID; }

        Shader* getShader();

        void setAlbedoTexture(const std::string& filename);
        const std::string& getAlbedoTextureFilename() const { return m_albedoTextureFilename; }
        int32_t getAlbedoTextureIndex() const { return m_albedoTextureIndex; }

        void setDiffuse(const glm::vec3& diffuse) { m_diffuse = diffuse; }
        const glm::vec3& getDiffuse() const { return m_diffuse; }
        glm::vec3& getDiffuse() { return m_diffuse; }

        void setSpecular(const glm::vec3& specular) { m_specular = specular; }
        const glm::vec3& getSpecular() const { return m_specular; }
        glm::vec3& getSpecular() { return m_specular; }

        void setShininess(float shininess) { m_shininess = shininess; }
        float getShininess() const { return m_shininess; }

        void loadUniforms();


        // TODO: Move those 2 methods away - Vertex and Instance Buffers Layout
        void updateVboDataLayout(VertexBufferLayout& layout) const;
        void updateInstanceDataLayout(VertexBufferLayout& layout) const;
    
    private:
        Texture2DArray* getAlbedoTextureArray();

    private:
        static const char* MATERIAL_ALBEDO_TEXTURE_NAME;

        std::string m_name;
        const std::string m_shaderName;
        uint32_t m_instanceID{0};
        Shader* m_shader{nullptr};

        glm::vec3 m_diffuse;
        glm::vec3 m_specular;
        float m_shininess{1.0f};
        std::string m_albedoTextureFilename;
        int32_t m_albedoTextureIndex{-1};

    };

} // namespace comet