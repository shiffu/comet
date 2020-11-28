#include <comet/spotLight.h>
#include <comet/shader.h>
#include <sstream>
#include <glm/trigonometric.hpp>
#include <comet/log.h>

// Must be the same value as in the glsl file
#define MAX_SPOT_LIGHTS 4

namespace comet
{
    uint16_t SpotLight::s_spotLightsCount = 0;

    void SpotLight::loadUniforms(Shader* shader)
    {
        if (m_index == 0)
        {
            shader->setUniform("active_spot_lights_count", (uint32_t)s_spotLightsCount);
        }

        if (m_index >= MAX_SPOT_LIGHTS)
        {
            CM_CORE_LOG_WARN("Exceeded the max number of Spot Lights allowed: {}", MAX_SPOT_LIGHTS);
            return;
        }

        std::stringstream ss;
        ss << "spot_lights[" << m_index << "].position";
        shader->setUniform(ss.str(), m_position); ss.str(""); ss.clear();

        ss << "spot_lights[" << m_index << "].direction";
        shader->setUniform(ss.str(), m_direction); ss.str(""); ss.clear();

        ss << "spot_lights[" << m_index << "].cutoff";
        shader->setUniform(ss.str(), glm::cos(m_cutoffAngle)); ss.str(""); ss.clear();

        ss << "spot_lights[" << m_index << "].outer_cutoff";
        shader->setUniform(ss.str(), glm::cos(m_outerCutoffAngle)); ss.str(""); ss.clear();

        ss << "spot_lights[" << m_index << "].ambient";
        shader->setUniform(ss.str(), m_ambient); ss.str(""); ss.clear();

        ss << "spot_lights[" << m_index << "].diffuse";
        shader->setUniform(ss.str(), m_diffuse); ss.str(""); ss.clear();

        ss << "spot_lights[" << m_index << "].specular";
        shader->setUniform(ss.str(), m_specular); ss.str(""); ss.clear();
    }

} // namespace comet
