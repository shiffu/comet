#include <comet/textureRegistry.h>

namespace comet
{
    
    Texture2D* TextureRegistry::getTexture2D(const std::string& filepath)
    {
        if (m_texture2DRegistry.find(filepath) == m_texture2DRegistry.end())
        {
            m_texture2DRegistry[filepath] = std::make_unique<Texture2D>(filepath.c_str());
        }

        return m_texture2DRegistry[filepath].get();
    }

    Texture2D* TextureRegistry::getGeneratedWhiteTexture2D()
    {
        if (m_generatedWhiteTexture2d == nullptr)
        {
            m_generatedWhiteTexture2d = Texture2D::generateWhiteTexture();
        }

        return m_generatedWhiteTexture2d.get();
    }
    
    Texture2DArray* TextureRegistry::getTexture2DArray(const std::string& uniqueName)
    {
        if (m_texture2DArrayRegistry.find(uniqueName) == m_texture2DArrayRegistry.end())
        {
            m_texture2DArrayRegistry[uniqueName] = std::make_unique<Texture2DArray>();
        }

        return m_texture2DArrayRegistry[uniqueName].get();
    }

} // namespace comet
