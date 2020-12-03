#pragma once

#include <string>
#include <vector>
#include <memory>

namespace comet
{

	class Texture
	{
	public:
		Texture() = default;
		Texture(uint32_t textureId)
			: m_textureId(textureId) {}
		virtual ~Texture() { cleanUp(); };

		Texture(const Texture& other) = delete;
		Texture& operator=(const Texture& other) = delete;
		Texture(Texture&& other);
		Texture& operator=(Texture&& other);

		void cleanUp();
        uint32_t getId() const { return m_textureId; }
		
		virtual void load() = 0;
		virtual void bind(uint32_t textureSlot = 0) const = 0;
		virtual void unbind() const = 0;

	protected:
		uint32_t m_textureId;
	};


    class Texture2D : public Texture
	{
	public:
		Texture2D(uint32_t textureId, uint32_t width, uint32_t height)	
			: Texture(textureId), m_width(width), m_height(height) {}
		Texture2D(const char* filename);
		~Texture2D() {};

		static std::unique_ptr<Texture2D> generateWhiteTexture();
		
		void load() override;
		void bind(uint32_t textureSlot = 0) const override;
		void unbind() const override;

        uint32_t getWidth() const { return m_width; }
        uint32_t getHeight() const { return m_height; }
        
	private:
        uint32_t m_width{0};
        uint32_t m_height{0};
		std::string m_filepath{};
	};


    class Texture2DArray : public Texture
	{
	public:
		Texture2DArray() {};
		~Texture2DArray() {};
		
		void load() override;
		void bind(uint32_t textureSlot = 0) const override;
		void unbind() const override;

		uint32_t addTexture2D(const char* filename);
		uint32_t getTextureCount() const { return m_filepaths.size(); }
		bool needToLoadTextureArray() const { return m_needToLoadTextureArray; }

        uint32_t getWidth() const { return m_width; }
        uint32_t getHeight() const { return m_height; }
        
	private:
        uint32_t m_width{0};
        uint32_t m_height{0};
		std::vector<std::string> m_filepaths{};
		bool m_needToLoadTextureArray{true};
	};

} // namespace comet
