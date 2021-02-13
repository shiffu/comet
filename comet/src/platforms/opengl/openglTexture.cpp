#include "openglTexture.h"

#include <comet/resourceManager.h>
#include <comet/log.h>
#include <comet/assert.h>

#include <fstream>

#include <stb/stb_image.h>
#include <glad/glad.h>

namespace comet
{


    OpenglTexture2D::OpenglTexture2D(uint32_t textureId)
        : m_textureId(textureId), m_isProxy{true} {}

    OpenglTexture2D::OpenglTexture2D(const char* filename /*= nullptr*/)
    {
        if (filename)
        {
            auto textureResourcePath = ResourceManager::getInstance().getResourcePath(ResourceType::TEXTURE, filename);
            m_filepath = textureResourcePath.string();
            load();
        }
    }

 	OpenglTexture2D::OpenglTexture2D(OpenglTexture2D&& other)
        : m_textureId(std::move(other.m_textureId)),
        m_width(std::move(other.m_width)),
        m_height(std::move(other.m_height))
    {
        other.m_textureId = 0;
    }

 	OpenglTexture2D::~OpenglTexture2D()
    {
        if (!m_isProxy)
            cleanUp();
    }

	OpenglTexture2D& OpenglTexture2D::operator=(OpenglTexture2D&& other)
    {
        if (&other == this)
        {
            return *this;
        }

        cleanUp();
        m_textureId = std::move(other.m_textureId);
        m_width = std::move(other.m_width);
        m_height = std::move(other.m_height);

        other.m_textureId = 0;
    }

    void OpenglTexture2D::makeItWhite()
    {
        cleanUp();
        glCreateTextures(GL_TEXTURE_2D, 1, &m_textureId);
        glBindTexture(GL_TEXTURE_2D, m_textureId);
        m_width = 1;
        m_height = 1;
        glTextureStorage2D(m_textureId, 1, GL_RGB8, m_width, m_height);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        uint32_t whitePixel = 0xFFFFFFFF;
        glTextureSubImage2D(m_textureId, 0, 0, 0, m_width, m_height, GL_RGB, GL_UNSIGNED_BYTE, &whitePixel);
    }

    void OpenglTexture2D::cleanUp()
    {
        if (m_textureId)
        {
            glDeleteTextures(1, &m_textureId);
            m_textureId = 0;
        }
    }

    // OpenglTexture2D
    void OpenglTexture2D::load()
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
            ASSERT(data, "Failed to load texture");
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

    void OpenglTexture2D::bind(uint32_t textureSlot /*= 0*/) const
    {
		glActiveTexture(GL_TEXTURE0 + textureSlot);
		glBindTexture(GL_TEXTURE_2D, m_textureId);
    }

    void OpenglTexture2D::unbind() const
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }


    // OpenglTexture2DArray
 	OpenglTexture2DArray::OpenglTexture2DArray(OpenglTexture2DArray&& other)
        : m_textureId(std::move(other.m_textureId)),
        m_width(std::move(other.m_width)),
        m_height(std::move(other.m_height)),
        m_filepaths(std::move(other.m_filepaths)),
        m_needToLoadTextureArray(std::move(other.m_needToLoadTextureArray))
    {
        other.m_textureId = 0;
    }

 	OpenglTexture2DArray::~OpenglTexture2DArray()
    {
       cleanUp();
    }

	OpenglTexture2DArray& OpenglTexture2DArray::operator=(OpenglTexture2DArray&& other)
    {
        if (&other == this)
        {
            return *this;
        }
        
        cleanUp();
        m_textureId = std::move(other.m_textureId);
        m_width = std::move(other.m_width);
        m_height = std::move(other.m_height);
        m_filepaths = std::move(other.m_filepaths);
        m_needToLoadTextureArray = std::move(other.m_needToLoadTextureArray);

        other.m_textureId = 0;
    }

    void OpenglTexture2DArray::cleanUp()
    {
        if (m_textureId)
        {
            glDeleteTextures(1, &m_textureId);
            m_textureId = 0;
        }
    }

	void OpenglTexture2DArray::load()
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

	void OpenglTexture2DArray::bind(uint32_t textureSlot /*= 0*/) const
    {
		glActiveTexture(GL_TEXTURE0 + textureSlot);
		glBindTexture(GL_TEXTURE_2D_ARRAY, m_textureId);
    }

	void OpenglTexture2DArray::unbind() const
    {
        glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
    }

    // All textures added to the texture array must have the same size
	uint32_t OpenglTexture2DArray::addTexture2D(const char* filename)
    {
        auto textureResourcePath = ResourceManager::getInstance().getResourcePath(ResourceType::TEXTURE, filename);
        m_filepaths.push_back(textureResourcePath.string());
        m_needToLoadTextureArray = true;
        
        return m_filepaths.size() - 1;
    }

} // namespace comet
