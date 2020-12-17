#pragma once

#include <entt/entt.hpp>
#include <comet/scene.h>
#include <comet/components.h>
#include <comet/assert.h>

namespace comet
{
    
    class Entity
    {
    public:
        Entity(entt::entity entityId, Scene* scene)
            : m_entityId(entityId) , m_scene(scene)
        {
            addComponent<TransformComponent>();
        }

        template<typename T, typename... Args>
        T& addComponent(Args&&... args)
        {
            ASSERT(!hasComponent<T>(), "Component already exists in this entity");
            return m_scene->m_registry.emplace<T>(m_entityId, std::forward<Args>(args)...);
        }

        template<typename T>
        void removeComponent(T comp)
        {
            ASSERT(hasComponent<T>(), "Component is not in this entity");
            m_scene->m_registry.remove<T>(m_entityId);
        }

        template<typename T>
        bool hasComponent()
        {
            return m_scene->m_registry.has<T>(m_entityId);
        }

        template<typename T>
        T& getComponent()
        {
            ASSERT(hasComponent<T>(), "Component is not in this entity");
            return m_scene->m_registry.get<T>(m_entityId);
        }

    private:
        entt::entity m_entityId{entt::null};
        Scene* m_scene{nullptr};
    };

} // namespace comet
