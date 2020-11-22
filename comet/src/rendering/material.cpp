#include <comet/material.h>
#include <comet/shaderRegistry.h>

namespace comet
{
    Shader* Material::getShader()
    {
        if (m_shader == nullptr)
        {
            m_shader = ShaderRegistry::getInstance().getShader(m_shaderName.c_str());
        }
        return m_shader;
    }
    
} // namespace comet
