#pragma once

#include <comet/light.h>

namespace comet
{

    class DirectionalLight : public Light
    {
    public:
        DirectionalLight(const glm::vec3& direction)
            : Light(), m_direction(direction) {}
        
        ~DirectionalLight() {}

        void loadUniforms(Shader* shader) override;

        void setDirection(const glm::vec3& dir) { m_direction = dir; }
        const glm::vec3& getDirection() const { return m_direction; }

    private:
        glm::vec3 m_direction{};
    };
    
} // namespace comet
