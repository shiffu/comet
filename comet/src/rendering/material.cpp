#include <comet/material.h>

namespace comet
{
        void Material::set(const std::string& uniformName, float value)
        {
            m_shader.setUniform(uniformName, value);
        }

        void Material::set(const std::string& uniformName, uint32_t value)
        {
            m_shader.setUniform(uniformName, value);
        }

        void Material::set(const std::string& uniformName, const glm::vec2& value)
        {
            m_shader.setUniform(uniformName, value);
        }

        void Material::set(const std::string& uniformName, const glm::vec3& value)
        {
            m_shader.setUniform(uniformName, value);
        }

        void Material::set(const std::string& uniformName, const glm::vec4& value)
        {
            m_shader.setUniform(uniformName, value);
        }

        void Material::set(const std::string& uniformName, const glm::mat4& value)
        {
            m_shader.setUniform(uniformName, value);
        }


} // namespace comet
