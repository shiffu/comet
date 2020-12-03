#include <glad/glad.h>
#include <comet/texture.h>
#include <core/resourceManager.h>
#include <comet/log.h>
#include <fstream>
#include <stb/stb_image.h>
#include <core/asserts.h>

namespace comet
{
 
    // Texture
 	Texture::Texture(Texture&& other)
    {
        m_textureId = other.m_textureId;
        other.m_textureId = 0;
    }

	Texture& Texture::operator=(Texture&& other)
    {
        cleanUp();
        m_textureId = other.m_textureId;
        other.m_textureId = 0;
    }

    void Texture::cleanUp()
    {
        if (m_textureId)
        {
            glDeleteTextures(1, &m_textureId);
            m_textureId = 0;
        }
    }

    Texture2D::Texture2D(const char* filename) : Texture()
    {
        auto textureResourcePath = ResourceManager::getInstance().getResourcePath(ResourceType::TEXTURE, filename);
        m_filepath = textureResourcePath.string();
        load();
    }

    std::unique_ptr<Texture2D> Texture2D::generateWhiteTexture()
    {
        uint32_t textureId;

        glCreateTextures(GL_TEXTURE_2D, 1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glTextureStorage2D(textureId, 1, GL_RGB8, 1, 1);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        uint32_t whitePixel = 0xFFFFFFFF;
        glTextureSubImage2D(textureId, 0, 0, 0, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &whitePixel);

        return std::make_unique<Texture2D>(textureId, 1, 1);
    }

    // Texture2D
    void Texture2D::load()
    {
        cleanUp();
		std::ifstream ifs(m_filepath);
		if (ifs.fail())
        {
            CM_CORE_LOG_ERROR("Failed loading texture file {}", m_filepath);
            return;
		}

		if (ifs)
        {
            int width, height, channels;
            stbi_set_flip_vertically_on_load(1);
            auto data = stbi_load(m_filepath.c_str(), &width, &height, &channels, 0);
            ASSERT(data);
            m_width = width;
            m_height = height;

            GLenum internalFormat, fileFormat;
            if (channels == 3)
            {
                internalFormat = GL_RGB8;
                fileFormat = GL_RGB;
            }
            else if (channels == 4)
            {  
                internalFormat = GL_RGBA8;
                fileFormat = GL_RGBA;
            }
            else
            {
                CM_CORE_LOG_ERROR("Image with {} channels is not a supported format (in file {})", channels, m_filepath);
                return;
            }

            glCreateTextures(GL_TEXTURE_2D, 1, &m_textureId);
            glBindTexture(GL_TEXTURE_2D, m_textureId);
            glTextureStorage2D(m_textureId, 1, internalFormat, m_width, m_height);
            
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            glTextureSubImage2D(m_textureId, 0, 0, 0, m_width, m_height, fileFormat, GL_UNSIGNED_BYTE, data);

            stbi_image_free(data);
        }
    }

    void Texture2D::bind(uint32_t textureSlot /*= 0*/) const
    {
		glActiveTexture(GL_TEXTURE0 + textureSlot);
		glBindTexture(GL_TEXTURE_2D, m_textureId);
    }

    void Texture2D::unbind() const
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }


    // Texture2DArray
	void Texture2DArray::load()
    {
        cleanUp();
        bool firstTextureFile{true};
        uint32_t referenceChannels;
        
        stbi_set_flip_vertically_on_load(1);

        uint32_t textureIndex{0};
        for (auto filepath : m_filepaths)
        {
            std::ifstream ifs(filepath);
            if (ifs.fail())
            {
                CM_CORE_LOG_ERROR("Failed loading texture file {}", filepath);
                return;
            }

            if (ifs)
            {
                int width, height, channels;
                GLenum internalFormat, fileFormat;
                auto data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
                ASSERT(data);

                // Creation, Allocation and Sampler Parameter setup is done only on the first loaded texture
                if (firstTextureFile)
                {
                    firstTextureFile = false;
                    m_width = width;
                    m_height = height;

                    glCreateTextures(GL_TEXTURE_2D_ARRAY, 1, &m_textureId);
                    glBindTexture(GL_TEXTURE_2D_ARRAY, m_textureId);

                    if (channels == 3)
                    {
                        internalFormat = GL_RGB8;
                        fileFormat = GL_RGB;
                    }
                    else if (channels == 4)
                    {  
                        internalFormat = GL_RGBA8;
                        fileFormat = GL_RGBA;
                    }
                    else
                    {
                        CM_CORE_LOG_ERROR("Image with {} channels is not a supported format (check file {})", channels, filepath);
                        return;
                    }

                    referenceChannels = channels;
                    glTextureStorage3D(m_textureId, 1, internalFormat, m_width, m_height, m_filepaths.size());

                    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
                    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
                    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                }
                
                if (m_width != width || m_height != height || channels != referenceChannels)
                {
                    CM_CORE_LOG_ERROR("All images must have the same size and number of channels ({} x {} x {}) (check file {})",
                                        m_width, m_height, referenceChannels, filepath);
                    return;
                }

                glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, textureIndex, m_width, m_height, 1, fileFormat, GL_UNSIGNED_BYTE, data);
                textureIndex++;

                stbi_image_free(data);
            }
        }

        m_needToLoadTextureArray = false;
    }

	void Texture2DArray::bind(uint32_t textureSlot /*= 0*/) const
    {
		glActiveTexture(GL_TEXTURE0 + textureSlot);
		glBindTexture(GL_TEXTURE_2D_ARRAY, m_textureId);
    }

	void Texture2DArray::unbind() const
    {
        glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
    }

    // All textures added to the texture array must have the same size
	uint32_t Texture2DArray::addTexture2D(const char* filename)
    {
        auto textureResourcePath = ResourceManager::getInstance().getResourcePath(ResourceType::TEXTURE, filename);
        m_filepaths.push_back(textureResourcePath.string());
        m_needToLoadTextureArray = true;
        
        return m_filepaths.size() - 1;
    }

} // namespace comet
