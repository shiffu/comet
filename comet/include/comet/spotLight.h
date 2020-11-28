#pragma once

#include <comet/light.h>
#include <vector>
#include <glm/glm.hpp>

namespace comet
{
    class SpotLight : public Light
    {
    public:
        SpotLight(const glm::vec3& pos)
            : Light(), m_position(pos) { m_index = s_spotLightsCount++; }
        ~SpotLight() {}

        void loadUniforms(Shader* shader) override;

        void setPosition(const glm::vec3& pos) { m_position = pos; }
        const glm::vec3& getPosition() const { return m_position; }

        void setDirection(const glm::vec3& dir) { m_direction = dir; }
        const glm::vec3& getDirection() const { return m_direction; }

        void setCutoffAngle(float cutoffAngle) { m_cutoffAngle = cutoffAngle; }
        float getCutoffAngle() const { return m_cutoffAngle; }

        void setOuterCutoffAngle(float cutoffAngle) { m_outerCutoffAngle = cutoffAngle; }
        float getOuterCutoffAngle() const { return m_outerCutoffAngle; }

    private:
        glm::vec3 m_position;
        glm::vec3 m_direction{0.0f, -1.0f, 0.0f};
        float m_cutoffAngle{glm::radians(30.0f)};
        float m_outerCutoffAngle{glm::radians(45.0f)};
        uint16_t m_index;
        static uint16_t s_spotLightsCount;
    };

} // namespace comet
