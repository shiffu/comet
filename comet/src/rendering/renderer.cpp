#include <comet/renderer.h>
#include <glad/glad.h>

namespace comet
{
    
    void Renderer::drawIndexed(unsigned int count)
    {
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, (void*)nullptr);
    }

} // namespace comet
