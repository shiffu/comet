#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <string>
#include <comet/shader.h>
#include <comet/log.h>
#include <comet/utils.h>

namespace comet
{

    Shader::Shader(const std::string& name) : m_name(name)
    {
        m_program = glCreateProgram();
    }

    Shader::~Shader()
    {
        if (m_program) {
            glDeleteProgram(m_program);
            m_program = 0;
        }
    }

    uint32_t Shader::getTypeHash() const
    {
        return utils::hashStr(m_name.c_str());
    }

    Shader::Type Shader::getShaderTypeFromSuffix(const std::string& suffix)
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

    void Shader::compileShaderFile(const std::string& filename, Type shaderType)
    {
        // Read the shader file
        std::ifstream shaderFile(filename, std::ifstream::in);
        if (shaderFile.fail())
        {
            perror(filename.c_str());
            CM_CORE_LOG_FATAL("Failed to open {}", filename);
        }

        std::string shaderSource = "";
        std::string line;
        while (std::getline(shaderFile, line))
        {
            shaderSource += line + "\n";
        }
        shaderFile.close();

        compileShaderSrc(shaderSource, shaderType);
    }

    void Shader::compileShaderSrc(const std::string& source, Type shaderType)
    {
        unsigned int glShaderType;
        switch (shaderType)
        {
        case Type::VERTEX:
            glShaderType = GL_VERTEX_SHADER;
            m_hasVertexShader = true;
            break;

        case Type::FRAGMENT:
            glShaderType = GL_FRAGMENT_SHADER;
            m_hasFragmentShader = true;
            break;

        case Type::GEOMETRY:
            glShaderType = GL_GEOMETRY_SHADER;
            break;
        
        default:
            CM_CORE_LOG_FATAL("Unmanaged shader type {}", shaderType);
            exit(EXIT_FAILURE);
            break;
        }

        unsigned int shader = glCreateShader(glShaderType);
        if (shader == 0)
        {
            CM_CORE_LOG_FATAL("Unable to create shader {}", shaderType);
            exit(EXIT_FAILURE);
        }

        const char* stringData = source.c_str();
        const char* const* shaderSrcPtr = (const char* const*)&stringData;
        glShaderSource(shader, 1, shaderSrcPtr, nullptr);

        // Compile the shader and check for errors
        glCompileShader(shader);
        int success = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (success == GL_FALSE)
        {
            // Get message length
            int maxLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

            // Get the log message
            std::vector<char> infoLog(maxLength);
            glGetShaderInfoLog(shader, maxLength, &maxLength, infoLog.data());

            // Don't leak the shader
            glDeleteShader(shader);

            CM_CORE_LOG_FATAL("Failed to compile shader {}: {}", m_name, std::string(infoLog.data()));
            exit(EXIT_FAILURE);
        }

        m_shaders[m_numShaders++] = shader;
    }

    void Shader::linkProgram()
    {
        // Check if there is at least a Vertext shader and a Fragment shader
        if (!m_hasVertexShader || !m_hasFragmentShader)
        {
            CM_CORE_LOG_FATAL("Both a Vertex and a Fragment shader must be provided to build a shader program");
            exit(EXIT_FAILURE);
        }

        // Attach our shaders to our program
        for (unsigned int i = 0; i < m_numShaders; i++)
        {
            glAttachShader(m_program, m_shaders[i]);
        }

        // Link our program
        glLinkProgram(m_program);

        int isLinked = 0;
        glGetProgramiv(m_program, GL_LINK_STATUS, (int*)&isLinked);
        if (isLinked == GL_FALSE)
        {
            int maxLength = 0;
            glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &maxLength);

            // The maxLength includes the NULL character
            std::vector<char> infoLog(maxLength);
            glGetProgramInfoLog(m_program, maxLength, &maxLength, infoLog.data());

            // We don't need the program anymore.
            glDeleteProgram(m_program);

            // Don't leak shaders either.
            for (unsigned int i = 0; i < m_numShaders; i++)
            {
                glDeleteShader(m_shaders[i]);
            }

            CM_CORE_LOG_FATAL("Failed to link the program {}: {}", m_name, std::string(infoLog.data()));
            exit(EXIT_FAILURE);
        }

        // Always detach and delete shaders after a successful link.
        for (unsigned int i = 0; i < m_numShaders; i++)
        {
            glDetachShader(m_program, m_shaders[i]);
        }

        for (unsigned int i = 0; i < m_numShaders; i++)
        {
            glDeleteShader(m_shaders[i]);
        }

        // In case we want to reuse this program with different shaders code.
        // TODO: Think more about this
        m_numShaders = 0;
    }

    void Shader::validateProgram()
    {
        if (m_program)
        {
            glValidateProgram(m_program);

            int isValid = 0;
            glGetProgramiv(m_program, GL_VALIDATE_STATUS, &isValid);
            if (isValid == GL_FALSE)
            {
                CM_CORE_LOG_FATAL("Failed to validate the program: {}", m_name);
                exit(EXIT_FAILURE);
            }
        }
    }

    int Shader::getUniformLocation(const std::string& name)
    {
        if (auto location = m_uniformLocationsCache.find(name); location != m_uniformLocationsCache.end())
        {
            return location->second;
        }

        int location = glGetUniformLocation(m_program, name.c_str());
        if (location == GL_INVALID_INDEX)
        {
            CM_CORE_LOG_WARN("Uniform {} not found in sharder program {}", name, m_name);
        }
        m_uniformLocationsCache[name] = location;

        return location;
    }

    void Shader::setUniform(const std::string& name, float value)
    {
        int location = getUniformLocation(name);
        glUniform1f(location, value);
    }

    void Shader::setUniform(const std::string& name, int value)
    {
        int location = getUniformLocation(name);
        glUniform1i(location, value);
    }

    void Shader::setUniform(const std::string& name, uint32_t value)
    {
        int location = getUniformLocation(name);
        glUniform1ui(location, value);
    }

    void Shader::setUniform(const std::string& name, const glm::vec2& value)
    {
        int location = getUniformLocation(name);
        glUniform2fv(location, 1, &value[0]);
    }

    void Shader::setUniform(const std::string& name, const glm::vec3& value)
    {
        int location = getUniformLocation(name);
        glUniform3fv(location, 1, &value[0]);
    }

    void Shader::setUniform(const std::string& name, const glm::vec4& value)
    {
        int location = getUniformLocation(name);
        glUniform4fv(location, 1, &value[0]);
    }

    void Shader::setUniform(const std::string& name, const glm::mat4& value)
    {
        int location = getUniformLocation(name);
        glUniformMatrix4fv(location, 1, false, &value[0][0]);
    }

    void Shader::setUniform(const std::string& name, uint32_t count, const int* values)
    {
        int location = getUniformLocation(name);
        glUniform1iv(location, count, (GLint*)values);
    }

    void Shader::setUniform(const std::string& name, uint32_t count, const uint32_t* values)
    {
        int location = getUniformLocation(name);
        glUniform1uiv(location, count, (GLuint*)values);
    }

    void Shader::setUniform(const std::string& name, uint32_t count, const float* values)
    {
        int location = getUniformLocation(name);
        glUniform1fv(location, count, (GLfloat*)values);
    }

    void Shader::setUniform(const std::string& name, uint32_t count, const glm::vec3* values)
    {
        int location = getUniformLocation(name);
        glUniform3fv(location, count, (GLfloat*)values);
    }

    void Shader::setUniform(const std::string& name, uint32_t count, const glm::vec4* values)
    {
        int location = getUniformLocation(name);
        glUniform4fv(location, count, (GLfloat*)values);
    }

    void Shader::bind() const { glUseProgram(m_program); }
    void Shader::unbind() const { glUseProgram(0); }

} // namespace comet
