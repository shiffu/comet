#pragma once

#include <string>

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
