#pragma once

#include <comet/texture.h>

#include <string>
#include <vector>

namespace comet
{
    
    class OpenglTexture2D : public Texture2D
	{
	public:
		
		OpenglTexture2D(uint32_t textureId);
		OpenglTexture2D(const char* filename = nullptr);
		virtual ~OpenglTexture2D();

		OpenglTexture2D(OpenglTexture2D&& other);
		OpenglTexture2D& operator=(OpenglTexture2D&& other);
		
		virtual void cleanUp() override;

		virtual void load() override;
		virtual void bind(uint32_t textureSlot = 0) const override;
		virtual void unbind() const override;

		virtual void makeItWhite() override;

        virtual uint32_t getWidth() const override { return m_width; }
        virtual uint32_t getHeight() const override { return m_height; }
        
	private:
		uint32_t m_textureId{0};
        uint32_t m_width{0};
        uint32_t m_height{0};
		std::string m_filepath{};
		bool m_isProxy{false};
	};

    class OpenglTexture2DArray : public Texture2DArray
	{
	public:
		OpenglTexture2DArray() {};
		virtual ~OpenglTexture2DArray();
		
		OpenglTexture2DArray(OpenglTexture2DArray&& other);
		OpenglTexture2DArray& operator=(OpenglTexture2DArray&& other);

		virtual void cleanUp() override;

		virtual void load() override;
		virtual void bind(uint32_t textureSlot = 0) const override;
		virtual void unbind() const override;

		virtual uint32_t addTexture2D(const char* filename) override;
		virtual uint32_t getTextureCount() const override { return m_filepaths.size(); }
		virtual bool needToLoadTextureArray() const override { return m_needToLoadTextureArray; }

        virtual uint32_t getWidth() const override { return m_width; }
        virtual uint32_t getHeight() const override { return m_height; }
        
	private:
		uint32_t m_textureId{0};
        uint32_t m_width{0};
        uint32_t m_height{0};
		std::vector<std::string> m_filepaths{};
		bool m_needToLoadTextureArray{true};
	};

} // namespace comet
