#pragma once

#include <comet/material.h>
#include <vector>
#include <glm/vec4.hpp>

namespace comet
{

    class PhongMaterial : public Material
    {
    public:
        PhongMaterial();
        PhongMaterial(const glm::vec3& diffuse);
        PhongMaterial(const glm::vec3& diffuse, const glm::vec3& specular);
        PhongMaterial(const glm::vec3& diffuse, const glm::vec3& specular, float shininess);
        ~PhongMaterial() {}

        void setDiffuse(const glm::vec3& diffuse) { m_diffuse = diffuse; }
        const glm::vec3& getDiffuse() const { return m_diffuse; }

        void setSpecular(const glm::vec3& specular) { m_specular = specular; }
        const glm::vec3& getSpecular() const { return m_specular; }

        void setShininess(float shininess) { m_shininess = shininess; }
        float getShininess() const { return m_shininess; }

        std::vector<PhongMaterial*>& getInstances() { return getMaterialInstances(); }

        void loadUniforms() override;
    
    private:
        std::vector<PhongMaterial*>& getMaterialInstances();
        void init();

    private:
        static const char* MATERIAL_NAME;
        static const char* SHADER_NAME;

    private:
        glm::vec3 m_diffuse;
        glm::vec3 m_specular;
        float m_shininess;
    };
    
} // namespace comet