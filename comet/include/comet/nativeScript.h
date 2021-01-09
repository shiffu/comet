#pragma once

#include <comet/entity.h>

namespace comet
{
    class PropertiesExposerInterface;

    class NativeScript
    {
        friend class Scene;
        friend class NativeScriptComponent;
        friend class PropertiesPanel;

    public:
        NativeScript() = default;
        virtual ~NativeScript() = default;

        PropertiesExposerInterface* getPropertiesExposer() { return m_propertiesExposer; }

    protected:
        virtual void onCreate() {}
        virtual void onUpdate(Entity& entity, double deltaTime) {}
        virtual void onDestroy() {}

        void setPropertiesExposer(PropertiesExposerInterface* propertiesExposer) { m_propertiesExposer = propertiesExposer; }

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
        
    private:
        PropertiesExposerInterface* m_propertiesExposer{nullptr};
    };
    
} // namespace comet
