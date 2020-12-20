#pragma once

#include <cstdint>
#include <memory>

namespace comet
{

	class Texture
	{
	public:
		Texture() = default;
		virtual ~Texture() = default;

		Texture(const Texture& other) = delete;
		Texture& operator=(const Texture& other) = delete;

		virtual void cleanUp() = 0;
		virtual void load() = 0;
		virtual void bind(uint32_t textureSlot = 0) const = 0;
		virtual void unbind() const = 0;
	};

    class Texture2D : public Texture
	{
	public:
		Texture2D(const char* filename = nullptr) {};
		virtual ~Texture2D() = default;

		static std::unique_ptr<Texture2D> create(const char* filename = nullptr);

		virtual void makeItWhite() = 0;

        virtual uint32_t getWidth() const = 0;
        virtual uint32_t getHeight() const = 0;
	};

    class Texture2DArray : public Texture
	{
	public:
		Texture2DArray() = default;
		virtual ~Texture2DArray() = default;

		static std::unique_ptr<Texture2DArray> create();

		virtual uint32_t addTexture2D(const char* filename) = 0;
		virtual uint32_t getTextureCount() const = 0;
		virtual bool needToLoadTextureArray() const = 0;

        virtual uint32_t getWidth() const = 0;
        virtual uint32_t getHeight() const = 0;
	};

} // namespace comet
