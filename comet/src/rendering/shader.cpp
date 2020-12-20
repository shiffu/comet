#include <comet/shader.h>
#include <comet/assert.h>
#include <comet/graphicApiConfig.h>
#include <platforms/opengl/openglShader.h>

namespace comet
{

    std::unique_ptr<Shader> Shader::create(const std::string& name)
    {
        switch (GraphicApiConfig::getApiImpl())
        {
            case GraphicApiConfig::API::OPENGL:
                return std::make_unique<OpenglShader>(name);
        }
        
        ASSERT(false, "Graphic API not supported for now!");
        return std::unique_ptr<OpenglShader>(nullptr);
    }

} // namespace comet
