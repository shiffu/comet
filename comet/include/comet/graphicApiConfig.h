#pragma once

namespace comet
{
    
    class GraphicApiConfig
    {
    public:
        enum class API
        {
            OPENGL = 0,
            DIRECTX11
        };

        static API getApiImpl();
    };

} // namespace comet
