#pragma once

#include <comet/shader.h>

namespace comet
{

    class Material
    {
    public:
        Material(const char* name) : m_name(name) {}

        const std::string& getName() const { return m_name; }
        const std::string& getShaderName() { return getShader()->getName(); }

        uint32_t getInstanceId() const { return m_instanceID; }

        virtual Shader* getShader() = 0;
        virtual void loadUniforms() = 0;

    protected:
        const std::string m_name;
        uint32_t m_instanceID{0};
        Shader* m_shader{nullptr};
    };

} // namespace comet