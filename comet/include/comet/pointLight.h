#pragma once

#include <comet/light.h>

namespace comet
{
    class PointLight : public Light
    {
    public:
        PointLight(const glm::vec3& pos)
            : Light(), m_position(pos) { m_index = s_pointLightsCount++; }
        ~PointLight() {}

        void loadUniforms(Shader* shader) override;

        void setPosition(const glm::vec3& pos) { m_position = pos; }
        const glm::vec3& getPosition() const { return m_position; }

        void setLinearAttenuation(float attenuation) { m_linearAttenuation = attenuation; }
        float getLinearAttenuation() const { return m_linearAttenuation; }

        void setQuadraticAttenuation(float attenuation) { m_quadraticAttenuation = attenuation; }
        float getQuadraticAttenuation() const { return m_quadraticAttenuation; }

    private:
        glm::vec3 m_position;
        float m_linearAttenuation{0.14};
        float m_quadraticAttenuation{0.07f};
        uint16_t m_index;
        static uint16_t s_pointLightsCount;
    };

} // namespace comet
