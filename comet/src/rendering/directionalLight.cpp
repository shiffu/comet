#include <comet/directionalLight.h>
#include <comet/shader.h>

namespace comet
{
    void DirectionalLight::loadUniforms(Shader* shader)
    {
        shader->setUniform("dir_light.direction", m_direction);
        shader->setUniform("dir_light.ambient", m_ambient);
        shader->setUniform("dir_light.diffuse", m_diffuse);
        shader->setUniform("dir_light.specular", m_specular);
    }

} // namespace comet
