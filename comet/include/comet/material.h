#pragma once

#include <comet/shader.h>

namespace comet
{

    class MaterialRegistry;
    class VertexBufferLayout;
    
    class Material
    {
        friend class MaterialRegistry;

    public:
        Material(const char* name, const char* shaderName) : m_name(name), m_shaderName(shaderName) {}

        const std::string& getName() const { return m_name; }
        const std::string& getShaderName() { return getShader()->getName(); }

        uint32_t getInstanceId() const { return m_instanceID; }

        virtual Shader* getShader();
        virtual uint32_t getTypeHash() const = 0;
        virtual void loadUniforms() = 0;

        // Vertex and Instance Buffers Layout
        virtual void updateVboDataLayout(VertexBufferLayout& layout) const = 0;
        virtual void updateInstanceDataLayout(VertexBufferLayout& layout) const = 0;

    protected:
        const std::string m_name;
        const std::string m_shaderName;
        uint32_t m_instanceID{0};
        Shader* m_shader{nullptr};
    };

} // namespace comet