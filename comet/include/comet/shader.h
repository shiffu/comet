#pragma once

#include <string>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace comet
{
    class Shader
    {
    public:
        enum class Type
        {
            NONE = 0,
            VERTEX,
            FRAGMENT
        };

        Shader();
        ~Shader();

        void compileShader(const std::string& filename, Type shaderType);
        void compileShaderSrc(const std::string& source, Type shaderType);

        void linkProgram();

        // Uniform methods
        int getUniformLocation(const std::string& name);
        void setUniform(const std::string& name, unsigned int value);
        void setUniform(const std::string& name, float value);
        void setUniform(const std::string& name, glm::vec2 value);
        void setUniform(const std::string& name, glm::vec3 value);
        void setUniform(const std::string& name, glm::mat4 value);

        void bind() const;
        void unbind() const;

    private:
        // Only Vertex and Fragment shaders are managed for now
        static const unsigned int NB_SHADERS = 2;

        unsigned int m_program = 0;
        unsigned int m_numShaders = 0;
        unsigned int m_shaders[NB_SHADERS];
    };
    
} // namespace comet
