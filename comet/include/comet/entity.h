#pragma once

#include <entt/entt.hpp>
#include <comet/scene.h>
#include <comet/components.h>
#include <comet/assert.h>

namespace comet
{

    class Scene;
    
    class Entity
    {
    public:
        Entity() = default;
        Entity(entt::entity entityId, Scene* scene)
            : m_entityId(entityId) , m_scene(scene)
        {
        }

        entt::entity getId() const { return m_entityId; }
        Scene* getScene() { return m_scene; }

        template<typename T, typename... Args>
        T& addComponent(Args&&... args)
        {
            ASSERT(!hasComponent<T>(), "Component already exists in this entity");
            return m_scene->m_registry.emplace<T>(m_entityId, std::forward<Args>(args)...);
        }

        template<typename T>
        void removeComponent()
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

        inline bool isValid() { return m_scene != nullptr && m_scene->m_registry.valid(m_entityId); }

        inline bool operator==(const Entity& other)
        {
            return (m_entityId == other.m_entityId && m_scene == other.m_scene);
        }

        inline bool operator!=(const Entity& other)
        {
            return !(*this == other);
        }

    private:
        entt::entity m_entityId{entt::null};
        Scene* m_scene{nullptr};
    };

} // namespace comet
