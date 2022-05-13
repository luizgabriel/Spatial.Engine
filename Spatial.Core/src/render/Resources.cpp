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

MaterialInstance createMaterialInstance(fl::Engine& engine, const SharedMaterial& material,
										const std::string_view name) noexcept
{
	return MaterialInstance{engine, material, material->createInstance(name.data())};
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

	// TODO: Can we move this "PixelBufferDescriptor" to the async continuation?
	auto bufferDescriptor = fl::Texture::PixelBufferDescriptor{
		imageData, size_t(width) * height * 4, fl::Texture::Format::RGBA, fl::Texture::Type::UBYTE,
		reinterpret_cast<fl::Texture::PixelBufferDescriptor::Callback>(&stbi_image_free)};

	auto texture = createTexture(engine, {static_cast<uint32_t>(width), static_cast<uint32_t>(height)},
								 fl::Texture::InternalFormat::RGBA8, usage, sampler);
	texture->setImage(engine, 0, std::move(bufferDescriptor));

	fl::Fence::waitAndDestroy(engine.createFence());

	return std::move(texture);
}

filament::VertexBuffer::Builder createVertexBufferBuilder(const FilameshFileHeader& header)
{
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

	return vbBuilder;
}

filament::VertexBuffer::BufferDescriptor createVertexBufferDescriptor(const FilameshFile& filamesh)
{
	return {filamesh.vertexData.data(), filamesh.header.vertexSize};
}

VertexBuffer createVertexBuffer(fl::Engine& engine, const FilameshFile& filamesh)
{
	auto vb = VertexBuffer{engine, createVertexBufferBuilder(filamesh.header).build(engine)};
	vb->setBufferAt(engine, 0, createVertexBufferDescriptor(filamesh));

	fl::Fence::waitAndDestroy(engine.createFence());

	return vb;
}

filament::IndexBuffer::Builder createIndexBufferBuilder(const FilameshFileHeader& header)
{
	return fl::IndexBuffer::Builder()
		.indexCount(header.indexCount)
		.bufferType(header.indexType ? fl::IndexBuffer::IndexType::USHORT : fl::IndexBuffer::IndexType::UINT);
}

filament::IndexBuffer::BufferDescriptor createIndexBufferDescriptor(const FilameshFile& filamesh)
{
	return {filamesh.indexData.data(), filamesh.header.indexSize};
}

IndexBuffer createIndexBuffer(fl::Engine& engine, const FilameshFile& filamesh)
{
	auto ib = IndexBuffer{engine, createIndexBufferBuilder(filamesh.header).build(engine)};
	ib->setBuffer(engine, createIndexBufferDescriptor(filamesh));

	fl::Fence::waitAndDestroy(engine.createFence());

	return ib;
}

Texture createDummyCubemap(filament::Engine& engine)
{
	auto texture = createTexture(engine, math::int2{1}, fl::Texture::InternalFormat::RGBA8, fl::Texture::Usage::DEFAULT,
								 filament::Texture::Sampler::SAMPLER_CUBEMAP);

	static const uint32_t pixel = 0xFF888888;
	auto buffer =
		filament::Texture::PixelBufferDescriptor(&pixel, 4, fl::Texture::Format::RGBA, fl::Texture::Type::UBYTE);
	const auto offsets = filament::Texture::FaceOffsets{};
	texture->setImage(engine, 0, std::move(buffer), offsets);

	fl::Fence::waitAndDestroy(engine.createFence());

	return texture;
}

MeshGeometries createMeshGeometries(const FilameshFile& filamesh)
{
	auto geometries = MeshGeometries(filamesh.parts.size());

	for (auto i = 0; i < filamesh.parts.size(); i++)
	{
		const auto& part = filamesh.parts[i];
		geometries.at(i) = MeshGeometry{part.offset, part.count};
	}

	return geometries;
}

Texture createTexture(filament::Engine& engine, const std::vector<uint32_t>& pixels, size_t width)
{
	auto dimensions = math::int2{width, pixels.size() / width};

	auto texture = createTexture(engine, dimensions, filament::Texture::InternalFormat::RGBA8,
								 filament::Texture::Usage::DEFAULT, filament::Texture::Sampler::SAMPLER_2D);

	auto size = dimensions.x * dimensions.y * sizeof(uint32_t);

	auto buffer = filament::Texture::PixelBufferDescriptor(pixels.data(), size, filament::Texture::Format::RGBA,
														   filament::Texture::Type::UBYTE);

	texture->setImage(engine, 0, std::move(buffer));

	fl::Fence::waitAndDestroy(engine.createFence());

	return texture;
}

} // namespace spatial::render