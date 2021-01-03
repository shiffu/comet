#pragma once

#include <comet/entity.h>

namespace comet
{

    class NativeScript
    {
        friend class Scene;

    public:
        NativeScript() = default;
        virtual ~NativeScript() = default;

    protected:
        virtual void onCreate() {}
        virtual void onUpdate(Entity& entity, double deltaTime) {}
        virtual void onDestroy() {}

        template<typename T>
        bool hasComponent(Entity& entity)
        {
            return entity.hasComponent<T>();
        }

        template<typename T>
        T& getComponent(Entity& entity)
        {
            return entity.getComponent<T>();
        }
    };
    
} // namespace comet
