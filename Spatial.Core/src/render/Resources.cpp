#include <spatial/render/Resources.h>

#include <filament/Fence.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace fl = filament;

namespace spatial::render
{

SwapChain createSwapChain(fl::Engine& engine, void* nativeWindowHandle) noexcept
{
	return SwapChain{engine, engine.createSwapChain(nativeWindowHandle)};
}

Renderer createRenderer(fl::Engine& engine) noexcept
{
	return Renderer{engine, engine.createRenderer()};
}

Scene createScene(fl::Engine& engine) noexcept
{
	return Scene{engine, engine.createScene()};
}

View createView(fl::Engine& engine) noexcept
{
	return View{engine, engine.createView()};
}

Camera createCamera(fl::Engine& engine, utils::Entity entity) noexcept
{
	return Camera{engine, entity};
}

Entity createEntity(fl::Engine& engine) noexcept
{
	return Entity{engine, utils::EntityManager::get().create()};
}

MaterialInstance createMaterialInstance(fl::Engine& engine, const fl::Material& material,
										const std::string_view name) noexcept
{
	return MaterialInstance{engine, material.createInstance(name.data())};
}

RenderTarget createRenderTarget(filament::Engine& engine, filament::Texture& color, filament::Texture& depth)
{
	auto target = fl::RenderTarget::Builder()
					  .texture(fl::RenderTarget::AttachmentPoint::COLOR, &color)
					  .texture(fl::RenderTarget::AttachmentPoint::DEPTH, &depth)
					  .build(engine);

	return RenderTarget{engine, target};
}

Material createMaterial(fl::Engine& engine, const uint8_t* data, size_t size)
{
	auto material = fl::Material::Builder().package(data, size).build(engine);
	return Material{engine, material};
}

Texture createTexture(filament::Engine& engine, math::int2 dimensions, fl::Texture::InternalFormat format,
					  fl::Texture::Usage usage, fl::Texture::Sampler sampler)
{
	auto texture = fl::Texture::Builder()
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

	auto bufferDescriptor = fl::Texture::PixelBufferDescriptor{
		imageData, size_t(width) * height * 4, fl::Texture::Format::RGBA, fl::Texture::Type::UBYTE,
		reinterpret_cast<fl::Texture::PixelBufferDescriptor::Callback>(&stbi_image_free)};

	auto texture = createTexture(engine, {static_cast<uint32_t>(width), static_cast<uint32_t>(height)},
								 fl::Texture::InternalFormat::RGBA8, usage, sampler);
	texture->setImage(engine, 0, std::move(bufferDescriptor));

	return std::move(texture);
}

VertexBuffer createVertexBuffer(fl::Engine& engine, const FilameshFile& filamesh)
{
	const auto& header = filamesh.header;

	const uint32_t FLAG_SNORM16_UV = 0x2;
	auto uvType = fl::VertexBuffer::AttributeType::HALF2;
	if (header.flags & FLAG_SNORM16_UV)
	{
		uvType = fl::VertexBuffer::AttributeType::SHORT2;
	}

	bool uvNormalized = header.flags & FLAG_SNORM16_UV;

	auto vbBuilder = fl::VertexBuffer::Builder()
						 .vertexCount(header.vertexCount)
						 .bufferCount(1)
						 .normalized(filament::TANGENTS)
						 .normalized(filament::COLOR)
						 .attribute(filament::POSITION, 0, fl::VertexBuffer::AttributeType::HALF4,
									header.offsetPosition, static_cast<uint8_t>(header.stridePosition))
						 .attribute(filament::TANGENTS, 0, fl::VertexBuffer::AttributeType::SHORT4,
									header.offsetTangents, static_cast<uint8_t>(header.strideTangents))
						 .attribute(filament::COLOR, 0, fl::VertexBuffer::AttributeType::UBYTE4, header.offsetColor,
									static_cast<uint8_t>(header.strideColor))
						 .attribute(filament::UV0, 0, uvType, header.offsetUV0, static_cast<uint8_t>(header.strideUV0))
						 .normalized(filament::UV0, uvNormalized);

	if (header.offsetUV1 != std::numeric_limits<uint32_t>::max()
		&& header.strideUV1 != std::numeric_limits<uint32_t>::max())
	{
		vbBuilder.attribute(filament::UV1, 0, uvType, header.offsetUV1, static_cast<uint8_t>(header.strideUV1))
			.normalized(filament::UV1, uvNormalized);
	}

	auto vb = VertexBuffer{engine, vbBuilder.build(engine)};

	auto vbBufferDescriptor = fl::VertexBuffer::BufferDescriptor(&filamesh.vertexData[0], header.vertexSize);
	vb->setBufferAt(engine, 0, std::move(vbBufferDescriptor));

	fl::Fence::waitAndDestroy(engine.createFence());

	return vb;
}

IndexBuffer createIndexBuffer(fl::Engine& engine, const FilameshFile& filamesh)
{
	const auto& header = filamesh.header;

	auto ibBuilder =
		fl::IndexBuffer::Builder()
			.indexCount(header.indexCount)
			.bufferType(header.indexType ? fl::IndexBuffer::IndexType::USHORT : fl::IndexBuffer::IndexType::UINT);

	auto ib = IndexBuffer{engine, ibBuilder.build(engine)};

	auto ibBufferDescriptor = fl::IndexBuffer::BufferDescriptor(&filamesh.indexData[0], header.indexSize);
	ib->setBuffer(engine, std::move(ibBufferDescriptor));

	fl::Fence::waitAndDestroy(engine.createFence());

	return ib;
}

Texture createDummyCubemap(filament::Engine& engine)
{
	auto* texture = filament::Texture::Builder()
						.width(1)
						.height(1)
						.levels(1)
						.format(filament::Texture::InternalFormat::RGBA8)
						.sampler(filament::Texture::Sampler::SAMPLER_CUBEMAP)
						.build(engine);

	static const uint32_t pixel = 0;
	auto buffer =
		filament::Texture::PixelBufferDescriptor(&pixel, 4, // 4 bytes in 1 RGBA pixel
												 filament::Texture::Format::RGBA, filament::Texture::Type::UBYTE);
	const auto offsets = filament::Texture::FaceOffsets{};
	texture->setImage(engine, 0, std::move(buffer), offsets);

	return Texture{engine, texture};
}

} // namespace spatial::render