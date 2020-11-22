#include <comet/shaderRegistry.h>
#include <experimental/filesystem>
#include <comet/log.h>
#include <core/resourceManager.h>

namespace comet
{
    namespace fs = std::experimental::filesystem;
    
    Shader* ShaderRegistry::getShader(const char* shaderName)
    {
        if (m_shaders.find(shaderName) == m_shaders.end())
        {
            m_shaders[shaderName] = new Shader(shaderName);
            compileLinkAndValidateShaders(m_shaders[shaderName]);
        }

        return m_shaders[shaderName];
    }

    void ShaderRegistry::compileLinkAndValidateShaders(Shader* shader)
    {
        static const fs::path shaderRootPath = ResourceManager::getInstance().getResourcePath(ResourceType::SHADER);
        static const std::string shaderExtension = ".glsl";

        // Find 'shaderName' shader files
        uint8_t shaderFoundCount{0};
        for (auto& p : fs::recursive_directory_iterator(shaderRootPath))
        {
            auto filenameWithoutExt = p.path().stem().string();
            auto const firstDotPos = filenameWithoutExt.find_first_of('.');
            const auto shaderName = filenameWithoutExt.substr(0, firstDotPos);

            if (shaderName != shader->getName())
            {
                continue;
            }

            auto extension = p.path().extension().string();
            if (extension != shaderExtension)
            {
                CM_CORE_LOG_WARN("file extension '{}' is not a proper GLSL Shader extension. Skipping file {}",
                                extension, p.path().filename().c_str());
                continue;
            }

            auto const pos = filenameWithoutExt.find_last_of('.');
            if (pos == std::string::npos)
            {
                CM_CORE_LOG_WARN("file {} does not have a proper shader suffix. Skipping this file",
                                p.path().filename().c_str());
                continue;
            }
            const auto shaderSuffix = filenameWithoutExt.substr(pos + 1);

            auto shaderType = shader->getShaderTypeFromSuffix(shaderSuffix);
            if (shaderType == Shader::Type::NONE)
            {
                CM_CORE_LOG_WARN("file suffix {} is not a supported shader suffix. Skipping file {}",
                                shaderSuffix, p.path().filename().c_str());
                continue;
            }

            CM_CORE_LOG_TRACE("Compiling shader file: {}, suffix: {}",
                            p.path().string(), shaderSuffix);

            shader->compileShaderFile(p.path().c_str(), shaderType);
            shaderFoundCount++;
        }

        if (shaderFoundCount >= 2)
        {
            // Link program
            CM_CORE_LOG_DEBUG("Linking shader program: {}", shader->getName());
            shader->linkProgram();

            // Validate program
            CM_CORE_LOG_DEBUG("Validating shader program: {}", shader->getName());
            shader->validateProgram();
        }
        else
        {
            CM_CORE_LOG_ERROR("At least 2 shader files ({} found) must be defined to build a shader program", shaderFoundCount);
        }
    }

} // namespace comet
