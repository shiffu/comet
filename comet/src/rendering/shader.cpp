#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <string>
#include <comet/shader.h>
#include <comet/log.h>

namespace comet
{

    Shader::Shader()
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

    void Shader::checkShaderTypeSupported(unsigned int shaderType) const
    {

        // Only Vertex and Fragment shaders are managed for the time being
        if (shaderType != GL_VERTEX_SHADER && shaderType != GL_GEOMETRY_SHADER && shaderType != GL_FRAGMENT_SHADER)
        {
            CM_CORE_LOG_FATAL("Only Vertex, Geometry and Fragment shaders are managed");
        }
    }

    void Shader::checkMaxShaderPerProgram() const
    {
        if (m_numShaders >= NB_SHADERS)
        {
            CM_CORE_LOG_FATAL("Exceeded max allowed shaders per program (3)");
        }
    }

    void Shader::compileShader(const std::string& filename, unsigned int shaderType)
    {
        checkShaderTypeSupported(shaderType);
        checkMaxShaderPerProgram();

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

    void Shader::compileShaderSrc(const std::string& source, unsigned int shaderType)
    {
        checkShaderTypeSupported(shaderType);
        checkMaxShaderPerProgram();

        unsigned int shader = glCreateShader(shaderType);
        if (shader == 0)
        {
            CM_CORE_LOG_FATAL("Unable to create shader {}", shaderType);
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

            CM_CORE_LOG_FATAL("Failed to compile shader: {}", std::string(infoLog.data()));
        }

        m_shaders[m_numShaders++] = shader;
    }

    void Shader::linkProgram()
    {
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

            CM_CORE_LOG_FATAL("Failed to link the program: {}", std::string(infoLog.data()));
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
        m_numShaders = 0;
    }

    int Shader::getUniformLocation(const std::string& name)
    {
        int location = glGetUniformLocation(m_program, name.c_str());
        if (location == GL_INVALID_INDEX)
        {
            CM_CORE_LOG_FATAL("Uniform {} not found in sharder program", name);
        }

        return location;
    }

    void Shader::setUniform(const std::string& name, float value)
    {
        int location = getUniformLocation(name);
        glUniform1f(location, value);
    }

    void Shader::setUniform(const std::string& name, unsigned int value)
    {
        int location = getUniformLocation(name);
        glUniform1i(location, value);
    }

    void Shader::setUniform(const std::string& name, glm::vec2 value)
    {
        int location = getUniformLocation(name);
        glUniform2fv(location, 1, &value[0]);
    }

    void Shader::setUniform(const std::string& name, glm::vec3 value)
    {
        int location = getUniformLocation(name);
        glUniform3fv(location, 1, &value[0]);
    }

    void Shader::setUniform(const std::string& name, glm::mat4 value)
    {
        int location = getUniformLocation(name);
        glUniformMatrix4fv(location, 1, false, &value[0][0]);
    }


    void Shader::bind() const { glUseProgram(m_program); }
    void Shader::unbind() const { glUseProgram(0); }

} // namespace comet
