#pragma once

#include <glm/glm.hpp>
#include <comet/vertex.h>
#include <glm/gtc/matrix_transform.hpp>

namespace comet
{
    
    struct TransformComponent
    {
        TransformComponent() = default;
        TransformComponent(const glm::vec3& pos)
        {
            translation = pos;
        }
        
        glm::mat4 getTransform() const
        {
            glm::mat4 identity(1.0f);
            auto rot = glm::rotate(identity, rotation.x, glm::vec3(1, 0, 0))
                * glm::rotate(identity, rotation.y, glm::vec3(0, 1, 0))
                * glm::rotate(identity, rotation.z, glm::vec3(0, 0, 1));

            return glm::translate(identity, translation)
                * rot
                * glm::scale(identity, scale);
        }

        glm::vec3 translation{0.0f};
        glm::vec3 scale{1.0f};
        glm::vec3 rotation{0.0f};
    };
    
    struct NameComponent
    {
        NameComponent() = default;
        NameComponent(const char* name) : name(name) {};
        NameComponent(const std::string& name) : name(name) {};

        std::string name{"Unamed"};
    };
    
    struct TagComponent
    {
        TagComponent() = default;
    };
    
    struct PlayerTagComponent
    {
        PlayerTagComponent() = default;
    };
    
    struct MeshComponent
    {
        MeshComponent() = default;
        MeshComponent(uint32_t meshTypeId) : meshTypeId(meshTypeId) {};

        uint32_t meshTypeId{0};
    };
    
    struct MaterialComponent
    {
        MaterialComponent() = default;
        MaterialComponent(uint32_t materialInstanceId)
            : materialInstanceId(materialInstanceId) {};

        uint32_t materialInstanceId{0};
    };

} // namespace comet
