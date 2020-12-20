#pragma once

#include <comet/singleton.h>
#include <comet/texture.h>

#include <unordered_map>
#include <string>

namespace comet
{
    class TextureRegistry : public Singleton<TextureRegistry>
    {
    public:
        Texture2D* getTexture2D(const std::string& filepath);
        Texture2D* getWhiteTexture2D();
        Texture2DArray* getTexture2DArray(const std::string& uniqueName);

    private:
        std::unordered_map<std::string, std::unique_ptr<Texture2D>> m_texture2DRegistry;
        std::unordered_map<std::string, std::unique_ptr<Texture2DArray>> m_texture2DArrayRegistry;
        std::unique_ptr<Texture2D> m_generatedWhiteTexture2d{nullptr};
    };

} // namespace comet
