#pragma once

#include <glm/vec3.hpp>

namespace comet
{
    class Shader;

    class Light
    {
    public:
        Light() {}
        virtual ~Light() {}

        virtual void loadUniforms(Shader* shader) = 0;

        void setAmbient(const glm::vec3& ambient) { m_ambient = ambient; }
        const glm::vec3& getAmbient() const { return m_ambient; }

        void setDiffuse(const glm::vec3& diffuse) { m_diffuse = diffuse; }
        const glm::vec3& getDiffuse() const { return m_diffuse; }

        void setSpecular(const glm::vec3& specular) { m_specular = specular; }
        const glm::vec3& getSpecular() const { return m_specular; }

    protected:
        glm::vec3 m_ambient{0.01f};
        glm::vec3 m_diffuse{0.9f};
        glm::vec3 m_specular{0.5f};
    };

} // namespace comet
