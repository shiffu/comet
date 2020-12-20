#include "vertexArray.h"
#include <comet/assert.h>
#include <comet/graphicApiConfig.h>
#include <platforms/opengl/openglVertexArray.h>

namespace comet
{

    std::unique_ptr<VertexArray> VertexArray::create()
    {
        switch (GraphicApiConfig::getApiImpl())
        {
            case GraphicApiConfig::API::OPENGL:
                return std::make_unique<OpenglVertexArray>();
        }
        
        ASSERT(false, "Graphic API not supported for now!");
        return std::unique_ptr<OpenglVertexArray>(nullptr);
    }
    
} // namespace comet
