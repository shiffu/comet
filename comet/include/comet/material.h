#pragma once

#include <comet/shader.h>

namespace comet
{

    class Material
    {
    public:
        Material(const char* name, Shader& shader) : m_name(name), m_shader(shader) {}

        Shader* getShader() const { return &m_shader; }
        const std::string& getName() const { return m_name; }
        
        void set(const std::string& uniformName, float value);
        void set(const std::string& uniformName, uint32_t value);
        void set(const std::string& uniformName, const glm::vec2& value);
        void set(const std::string& uniformName, const glm::vec3& value);
        void set(const std::string& uniformName, const glm::vec4& value);
        void set(const std::string& uniformName, const glm::mat4& value);

    private:
        Shader& m_shader;
        const std::string m_name;
    };

} // namespace comet
