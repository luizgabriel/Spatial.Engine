#include <spatial/render/ResourceLoaders.h>

#include <filament/Fence.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <sstream>
#include <math/vec2.h>
#include <math/vec3.h>

using namespace filament::math;
using namespace std::string_literals;
namespace fs = std::filesystem;
namespace fl = filament;

namespace std
{

template <typename ValueType>
istream& operator>>(istream& stream, filament::math::details::TVec2<ValueType>& vector)
{
	stream.read(reinterpret_cast<char*>(&vector[0]), sizeof(ValueType));
	stream.read(reinterpret_cast<char*>(&vector[1]), sizeof(ValueType));

	return stream;
}

template <typename ValueType>
istream& operator>>(istream& stream, filament::math::details::TVec3<ValueType>& vector)
{
	stream.read(reinterpret_cast<char*>(&vector[0]), sizeof(ValueType));
	stream.read(reinterpret_cast<char*>(&vector[1]), sizeof(ValueType));
	stream.read(reinterpret_cast<char*>(&vector[2]), sizeof(ValueType));

	return stream;
}

istream& operator>>(istream& stream, filament::Box& box)
{
	return stream >> box.center >> box.halfExtent;
}

istream& operator>>(istream& stream, spatial::FilameshFilePart& part)
{
	stream.read(reinterpret_cast<char*>(&part.offset), 4);
	stream.read(reinterpret_cast<char*>(&part.count), 4);
	stream.read(reinterpret_cast<char*>(&part.minIndex), 4);
	stream.read(reinterpret_cast<char*>(&part.maxIndex), 4);
	stream.read(reinterpret_cast<char*>(&part.materialID), 4);

	stream >> part.boundingBox;

	return stream;
}

istream& operator>>(istream& stream, spatial::FilameshFileHeader& header)
{
	char magic[9];
	stream.read(magic, 8);
	magic[8] = '\0';

	if (strcmp(magic, "FILAMESH") != 0)
	{
		stream.setstate(ios_base::failbit);
		return stream;
	}

	stream.read(reinterpret_cast<char*>(&header.version), 4);
	stream.read(reinterpret_cast<char*>(&header.partsCount), 4);

	stream >> header.aabb;

	stream.read(reinterpret_cast<char*>(&header.flags), 4);
	stream.read(reinterpret_cast<char*>(&header.offsetPosition), 4);
	stream.read(reinterpret_cast<char*>(&header.stridePosition), 4);
	stream.read(reinterpret_cast<char*>(&header.offsetTangents), 4);
	stream.read(reinterpret_cast<char*>(&header.strideTangents), 4);
	stream.read(reinterpret_cast<char*>(&header.offsetColor), 4);
	stream.read(reinterpret_cast<char*>(&header.strideColor), 4);
	stream.read(reinterpret_cast<char*>(&header.offsetUV0), 4);
	stream.read(reinterpret_cast<char*>(&header.strideUV0), 4);
	stream.read(reinterpret_cast<char*>(&header.offsetUV1), 4);
	stream.read(reinterpret_cast<char*>(&header.strideUV1), 4);
	stream.read(reinterpret_cast<char*>(&header.vertexCount), 4);
	stream.read(reinterpret_cast<char*>(&header.vertexSize), 4);
	stream.read(reinterpret_cast<char*>(&header.indexType), 4);
	stream.read(reinterpret_cast<char*>(&header.indexCount), 4);
	stream.read(reinterpret_cast<char*>(&header.indexSize), 4);

	return stream;
}

} // namespace std

namespace spatial
{

Material createMaterial(fl::Engine& engine, const std::string& resourceData)
{
	auto material = fl::Material::Builder().package(resourceData.data(), resourceData.size()).build(engine);
	return Material{engine, material};
}

Texture createTexture(filament::Engine& engine, std::pair<std::uint32_t, std::uint32_t> dimensions,
					  fl::Texture::InternalFormat format, fl::Texture::Usage usage, fl::Texture::Sampler sampler)
{
	auto texture = fl::Texture::Builder()
					   .width(dimensions.first)
					   .height(dimensions.second)
					   .levels(1)
					   .usage(usage)
					   .sampler(sampler)
					   .format(format)
					   .build(engine);

	return Texture{engine, texture};
}

Texture createTexture(fl::Engine& engine, const std::string& resourceData, fl::Texture::Usage usage, fl::Texture::Sampler sampler)
{
	int width, height, n;
	const auto data = stbi_load_from_memory(reinterpret_cast<stbi_uc const*>(resourceData.data()), resourceData.size(),
											&width, &height, &n, 4);

	auto bufferDescriptor = fl::Texture::PixelBufferDescriptor{
		data, size_t(width) * height * 4, fl::Texture::Format::RGBA, fl::Texture::Type::UBYTE,
		reinterpret_cast<fl::Texture::PixelBufferDescriptor::Callback>(&stbi_image_free)};

	auto texture = createTexture(engine, {static_cast<uint32_t>(width), static_cast<uint32_t>(height)},
								 fl::Texture::InternalFormat::RGBA8, usage, sampler);
	texture->setImage(engine, 0, std::move(bufferDescriptor));

	return std::move(texture);
}

VertexBuffer createVertexBuffer(fl::Engine& engine, const FilameshFileHeader& header, const std::vector<char>& vertices)
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

	if (header.offsetUV1 != std::numeric_limits<uint32_t>::max() &&
		header.strideUV1 != std::numeric_limits<uint32_t>::max())
	{
		vbBuilder.attribute(filament::UV1, 0, uvType, header.offsetUV1, static_cast<uint8_t>(header.strideUV1))
			.normalized(filament::UV1, uvNormalized);
	}

	auto vb = VertexBuffer{engine, vbBuilder.build(engine)};

	auto vbBufferDescriptor = fl::VertexBuffer::BufferDescriptor(&vertices[0], header.vertexSize);
	vb->setBufferAt(engine, 0, std::move(vbBufferDescriptor));

	fl::Fence::waitAndDestroy(engine.createFence());

	return vb;
}

IndexBuffer createIndexBuffer(fl::Engine& engine, const FilameshFileHeader& header, const std::vector<char>& indices)
{
	auto ibBuilder =
		fl::IndexBuffer::Builder()
			.indexCount(header.indexCount)
			.bufferType(header.indexType ? fl::IndexBuffer::IndexType::USHORT : fl::IndexBuffer::IndexType::UINT);

	auto ib = IndexBuffer{engine, ibBuilder.build(engine)};

	auto ibBufferDescriptor = fl::IndexBuffer::BufferDescriptor(&indices[0], header.indexSize);
	ib->setBuffer(engine, std::move(ibBufferDescriptor));

	fl::Fence::waitAndDestroy(engine.createFence());

	return ib;
}

FilameshFile createFilamesh(fl::Engine& engine, const std::string& resourceData)
{
	auto stream = std::istringstream{resourceData};
	auto header = FilameshFileHeader{};
	stream >> header;

	auto mesh = FilameshFile{std::move(header)};

	auto vertices = std::vector<char>(header.vertexSize);
	stream.read(&vertices[0], header.vertexSize);
	mesh.setVertexBuffer(toShared(createVertexBuffer(engine, header, vertices)));

	auto indices = std::vector<char>(header.indexSize);
	stream.read(&indices[0], header.indexSize);
	mesh.setIndexBuffer(toShared(createIndexBuffer(engine, header, indices)));

	for (size_t i = 0; i < mesh.getPartsCount(); i++)
	{
		stream >> mesh.getParts().at(i);
	}

	uint32_t materialCount;
	stream.read(reinterpret_cast<char*>(&materialCount), 4);

	for (size_t i = 0; i < header.partsCount; i++)
	{
		uint32_t nameLength;
		stream.read(reinterpret_cast<char*>(&nameLength), 4);

		std::getline(stream, mesh.getParts().at(i).materialName, '\0');
	}

	return std::move(mesh);
}

} // namespace spatial
