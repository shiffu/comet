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
        Shader();
        ~Shader();

        void compileShader(const std::string& filename, unsigned int shaderType);
        void compileShaderSrc(const std::string& source, unsigned int shaderType);

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
        // Only Vertex, Geometry and Fragment shaders are managed for now
        static const unsigned int NB_SHADERS = 3;

        void checkShaderTypeSupported(unsigned int shaderType) const;
        void checkMaxShaderPerProgram() const;

        unsigned int m_program = 0;
        unsigned int m_numShaders = 0;
        unsigned int m_shaders[NB_SHADERS];
    };
    
} // namespace comet
