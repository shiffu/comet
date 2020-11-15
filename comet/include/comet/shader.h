#pragma once

#include <string>
#include <unordered_map>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
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
            FRAGMENT,
            GEOMETRY
        };

        Shader();
        Shader(const std::string& name) : m_name(name) {};
        ~Shader();

        const std::string& getName() const { return m_name; }

        Shader::Type getShaderTypeFromSuffix(const std::string& suffix);

        void compileShaderFile(const std::string& filename, Type shaderType);
        void compileShaderSrc(const std::string& source, Type shaderType);

        void linkProgram();
        void validateProgram();

        void setViewProjectionMatrixName(const std::string& matrixName) { m_viewProjectionMatrixName = matrixName; }
        const std::string& getViewProjectionMatrixName() const { return m_viewProjectionMatrixName; }

        // Uniform methods
        // scalars
        int getUniformLocation(const std::string& name);
        void setUniform(const std::string& name, uint32_t value);
        void setUniform(const std::string& name, float value);
        void setUniform(const std::string& name, const glm::vec2& value);
        void setUniform(const std::string& name, const glm::vec3& value);
        void setUniform(const std::string& name, const glm::vec4& value);
        void setUniform(const std::string& name, const glm::mat4& value);

        // arrays
        void setUniform(const std::string& name, uint32_t count, const uint32_t* values);
        void setUniform(const std::string& name, uint32_t count, const glm::vec4* values);

        void bind() const;
        void unbind() const;

    private:
        std::string m_name{"Standard"};
        
        // Only Vertex, Fragment and Geometry shaders are managed for now
        static const unsigned int NB_SHADERS = 3;

        uint32_t m_program = 0;
        uint32_t m_numShaders = 0;
        uint32_t m_shaders[NB_SHADERS];
        std::unordered_map<std::string, int> m_uniformLocationsCache;

        std::string m_viewProjectionMatrixName{"vp_matrix"};
        bool m_hasVertexShader{false};
        bool m_hasFragmentShader{false};
    };
    
} // namespace comet
