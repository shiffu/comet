#pragma once
#include <comet/scene.h>

namespace comet
{

    class SceneSerializer
    {
    public:
        SceneSerializer() = delete;

        static bool serialize(Scene& scene);
        static bool deserialize(Scene& scene, const char* sceneFilename);
    };
    
} // namespace comet
