#pragma once

#include <comet/singleton.h>
#include <comet/shader.h>
#include <unordered_map>
#include <memory>

namespace comet
{
    class ShaderRegistry : public Singleton<ShaderRegistry>
    {
    public:
        Shader* getShader(const char* shaderName);

    private:
        void compileLinkAndValidateShaders(Shader* shader);

    private:
        std::unordered_map<std::string, std::unique_ptr<Shader>> m_shaders;
    };

} // namespace comet
