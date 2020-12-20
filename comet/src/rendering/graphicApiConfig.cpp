#include <comet/graphicApiConfig.h>

namespace comet
{
    
    GraphicApiConfig::API GraphicApiConfig::getApiImpl()
    {
        return API::OPENGL;
    }

} // namespace comet
