#include <filament/Fence.h>
#include <spatial/graphics/TextureResources.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace spatial::graphics
{

Texture createTexture(filament::Engine& engine, math::uvec2 dimensions, filament::Texture::InternalFormat format,
					  filament::Texture::Usage usage, filament::Texture::Sampler sampler)
{
	auto texture = filament::Texture::Builder()
					   .width(dimensions.x)
					   .height(dimensions.y)
					   .levels(1)
					   .usage(usage)
					   .sampler(sampler)
					   .format(format)
					   .build(engine);

	return Texture{engine, texture};
}

Texture createTexture(filament::Engine& engine, const uint8_t* data, size_t size, filament::Texture::Usage usage,
					  filament::Texture::Sampler sampler)
{
	int width, height, n;
	const auto* imageData = stbi_load_from_memory(data, static_cast<int>(size), &width, &height, &n, 4);

	// TODO: Can we move this "PixelBufferDescriptor" to the async continuation?
	auto bufferDescriptor = filament::Texture::PixelBufferDescriptor{
		imageData, size_t(width * height * 4), filament::Texture::Format::RGBA, filament::Texture::Type::UBYTE,
		reinterpret_cast<filament::Texture::PixelBufferDescriptor::Callback>(&stbi_image_free)};

	auto texture = createTexture(engine, {static_cast<uint32_t>(width), static_cast<uint32_t>(height)},
								 filament::Texture::InternalFormat::RGBA8, usage, sampler);
	texture->setImage(engine, 0, std::move(bufferDescriptor));

	filament::Fence::waitAndDestroy(engine.createFence());

	return texture;
}

Texture createTexture(filament::Engine& engine, const std::vector<uint32_t>& pixels, size_t width)
{
	auto dimensions = math::uvec2{width, pixels.size() / width};

	auto texture = createTexture(engine, dimensions, filament::Texture::InternalFormat::RGBA8,
								 filament::Texture::Usage::DEFAULT, filament::Texture::Sampler::SAMPLER_2D);

	auto size = dimensions.x * dimensions.y * sizeof(uint32_t);

	auto buffer = filament::Texture::PixelBufferDescriptor(pixels.data(), size, filament::Texture::Format::RGBA,
														   filament::Texture::Type::UBYTE);

	texture->setImage(engine, 0, std::move(buffer));

	filament::Fence::waitAndDestroy(engine.createFence());

	return texture;
}

Texture createDummyCubemap(filament::Engine& engine)
{
	auto texture = createTexture(engine, math::uvec2{1}, filament::Texture::InternalFormat::RGBA8,
								 filament::Texture::Usage::DEFAULT, filament::Texture::Sampler::SAMPLER_CUBEMAP);

	static const uint32_t pixel = 0xFF888888;
	auto buffer = filament::Texture::PixelBufferDescriptor(&pixel, 4, filament::Texture::Format::RGBA,
														   filament::Texture::Type::UBYTE);
	const auto offsets = filament::Texture::FaceOffsets{};
	texture->setImage(engine, 0, std::move(buffer), offsets);

	filament::Fence::waitAndDestroy(engine.createFence());

	return texture;
}

} // namespace spatial::graphics