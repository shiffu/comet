#include <comet/pointLight.h>
#include <comet/shader.h>
#include <sstream>
#include <comet/log.h>

// Must be the same value as in the glsl file
#define MAX_POINT_LIGHTS 4

namespace comet
{
    uint16_t PointLight::s_pointLightsCount = 0;

    void PointLight::loadUniforms(Shader* shader)
    {
        if (m_index == 0)
        {
            shader->setUniform("active_point_lights_count", (uint32_t)s_pointLightsCount);
        }

        if (m_index >= MAX_POINT_LIGHTS)
        {
            CM_CORE_LOG_WARN("Exceeded the max number of Point Lights allowed: {}", MAX_POINT_LIGHTS);
            return;
        }

        std::stringstream ss;
        ss << "point_lights[" << m_index << "].position";
        shader->setUniform(ss.str(), m_position); ss.str(""); ss.clear();
        
        ss << "point_lights[" << m_index << "].ambient";
        shader->setUniform(ss.str(), m_ambient); ss.str(""); ss.clear();

        ss << "point_lights[" << m_index << "].diffuse";
        shader->setUniform(ss.str(), m_diffuse); ss.str(""); ss.clear();

        ss << "point_lights[" << m_index << "].specular";
        shader->setUniform(ss.str(), m_specular); ss.str(""); ss.clear();

        ss << "point_lights[" << m_index << "].linear_attenuation";
        shader->setUniform(ss.str(), m_linearAttenuation); ss.str(""); ss.clear();

        ss << "point_lights[" << m_index << "].quadratic_attenuation";
        shader->setUniform(ss.str(), m_quadraticAttenuation); ss.str(""); ss.clear();
    }

} // namespace comet
