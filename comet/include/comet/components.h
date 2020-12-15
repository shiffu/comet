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
            transform = glm::translate(transform, pos);
        }
        
        void move(const glm::vec3& pos) { transform = glm::translate(transform, pos); }
        void rotate(float angle, const glm::vec3& axis) { transform = glm::rotate(transform, angle, axis); }
        void scale(const glm::vec3& values) { transform = glm::scale(transform, values); }

        glm::mat4 transform{1.0f};
    };
    
    struct MeshComponent
    {
        MeshComponent() = default;
        MeshComponent(uint32_t meshTypeId) : meshTypeId(meshTypeId) {};
        MeshComponent(uint32_t meshTypeId, uint32_t materialTypeId, uint32_t materialInstanceId)
            : meshTypeId(meshTypeId), materialTypeId(materialTypeId), materialInstanceId(materialInstanceId) {};

        uint32_t meshTypeId{0};
        uint32_t materialTypeId{0};
        uint32_t materialInstanceId{0};
    };

} // namespace comet
