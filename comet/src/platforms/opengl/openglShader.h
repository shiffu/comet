#pragma once

#include <comet/shader.h>

namespace comet
{

    class OpenglShader : public Shader
    {
    public:
        OpenglShader(const std::string& name);
        ~OpenglShader();

        // Compile & Link Shader Program
        virtual void compileShaderFile(const std::string& filename, Type shaderType) override;
        virtual void compileShaderSrc(const std::string& source, Type shaderType) override;
        virtual void linkProgram() override;
        virtual void validateProgram() override;

        // Uniform methods
        // scalars
        virtual int getUniformLocation(const std::string& name) override;
        virtual void setUniform(const std::string& name, int value) override;
        virtual void setUniform(const std::string& name, uint32_t value) override;
        virtual void setUniform(const std::string& name, float value) override;
        virtual void setUniform(const std::string& name, const glm::vec2& value) override;
        virtual void setUniform(const std::string& name, const glm::vec3& value) override;
        virtual void setUniform(const std::string& name, const glm::vec4& value) override;
        virtual void setUniform(const std::string& name, const glm::mat4& value) override;

        // arrays
        virtual void setUniform(const std::string& name, uint32_t count, const int* values) override;
        virtual void setUniform(const std::string& name, uint32_t count, const uint32_t* values) override;
        virtual void setUniform(const std::string& name, uint32_t count, const float* values) override;
        virtual void setUniform(const std::string& name, uint32_t count, const glm::vec3* values) override;
        virtual void setUniform(const std::string& name, uint32_t count, const glm::vec4* values) override;

        virtual void bind() const override;
        virtual void unbind() const override;

    private:
        // Only Vertex, Fragment and Geometry shaders are managed for now
        static const unsigned int NB_SHADERS = 3;

        uint32_t m_program = 0;
        uint32_t m_numShaders = 0;
        uint32_t m_shaders[NB_SHADERS];
        std::unordered_map<std::string, int> m_uniformLocationsCache;

        bool m_hasVertexShader{false};
        bool m_hasFragmentShader{false};
    };

} // namespace comet
