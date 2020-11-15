#pragma once

#include <core/singleton.h>
#include <comet/shader.h>
#include <unordered_map>

namespace comet
{
    class ShaderRegistry : public Singleton<ShaderRegistry>
    {
    public:
        Shader* getShader(const char* shaderName);

    private:
        void compileLinkAndValidateShaders(Shader* shader);

    private:
        std::unordered_map<std::string, Shader*> m_shaders;
    };

} // namespace comet
