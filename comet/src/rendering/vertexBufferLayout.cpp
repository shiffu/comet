#include "vertexBufferLayout.h"
#include <comet/assert.h>
#include <comet/graphicApiConfig.h>
#include <platforms/opengl/openglVertexBufferLayout.h>

namespace comet
{

    std::unique_ptr<VertexBufferLayout> VertexBufferLayout::create()
    {
        switch (GraphicApiConfig::getApiImpl())
        {
            case GraphicApiConfig::API::OPENGL:
                return std::make_unique<OpenglVertexBufferLayout>();
        }
        
        ASSERT(false, "Graphic API not supported for now!");
        return std::unique_ptr<OpenglVertexBufferLayout>(nullptr);
    }

} // namespace comet
