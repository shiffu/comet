#include <comet/scene.h>
#include <comet/entity.h>

namespace comet
{
    
    Entity Scene::createEntity()
    {
        Entity entity{ m_registry.create(), this};
        return entity;
    }

} // namespace comet
