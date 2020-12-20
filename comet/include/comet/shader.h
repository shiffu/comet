#pragma once

#include <comet/utils.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include <cstdint>
#include <string>
#include <memory>
#include <unordered_map>

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

        Shader(const std::string& name) : m_name(name) {};
        virtual ~Shader() {};

        static std::unique_ptr<Shader> create(const std::string& name);

        uint32_t getTypeHash() const { return utils::hashStr(m_name.c_str()); }
        const std::string& getName() const { return m_name; }

        Shader::Type getShaderTypeFromSuffix(const std::string& suffix)
        {
            if (suffix == "vs")
            {
                return Type::VERTEX;
            }
            else if (suffix == "fs")
            {
                return Type::FRAGMENT;
            }
            else if (suffix == "gs")
            {
                return Type::GEOMETRY;
            }

            return Type::NONE;
        }

        void setViewProjectionMatrixName(const std::string& matrixName) { m_viewProjectionMatrixName = matrixName; }
        const std::string& getViewProjectionMatrixName() const { return m_viewProjectionMatrixName; }

        void setViewMatrixName(const std::string& matrixName) { m_viewMatrixName = matrixName; }
        const std::string& getViewMatrixName() const { return m_viewMatrixName; }

        void setProjectionMatrixName(const std::string& matrixName) { m_projectionMatrixName = matrixName; }
        const std::string& getProjectionMatrixName() const { return m_projectionMatrixName; }

        // Compile & Link Shader Program
        virtual void compileShaderFile(const std::string& filename, Type shaderType) = 0;
        virtual void compileShaderSrc(const std::string& source, Type shaderType) = 0;
        virtual void linkProgram() = 0;
        virtual void validateProgram() = 0;

        // Uniform methods
        // scalars
        virtual int getUniformLocation(const std::string& name) = 0;
        virtual void setUniform(const std::string& name, int value) = 0;
        virtual void setUniform(const std::string& name, uint32_t value) = 0;
        virtual void setUniform(const std::string& name, float value) = 0;
        virtual void setUniform(const std::string& name, const glm::vec2& value) = 0;
        virtual void setUniform(const std::string& name, const glm::vec3& value) = 0;
        virtual void setUniform(const std::string& name, const glm::vec4& value) = 0;
        virtual void setUniform(const std::string& name, const glm::mat4& value) = 0;

        // arrays
        virtual void setUniform(const std::string& name, uint32_t count, const int* values) = 0;
        virtual void setUniform(const std::string& name, uint32_t count, const uint32_t* values) = 0;
        virtual void setUniform(const std::string& name, uint32_t count, const float* values) = 0;
        virtual void setUniform(const std::string& name, uint32_t count, const glm::vec3* values) = 0;
        virtual void setUniform(const std::string& name, uint32_t count, const glm::vec4* values) = 0;

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

    protected:
        std::string m_name{};
        std::string m_viewProjectionMatrixName{"vp_matrix"};
        std::string m_viewMatrixName{"view_matrix"};
        std::string m_projectionMatrixName{"projection_matrix"};
    };
    
} // namespace comet
