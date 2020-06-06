#include <spatial/render/ResourceLoaders.h>
#include <spatial/common/Exceptions.h>
#include <spatial/core/Asset.h>

#include <filament/Fence.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <fstream>
#include <string>
#include <fmt/format.h>

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

istream& operator>>(istream& stream, spatial::MeshPart& part)
{
	stream.read(reinterpret_cast<char*>(&part.offset), 4);
	stream.read(reinterpret_cast<char*>(&part.indexCount), 4);
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
	stream.read(reinterpret_cast<char*>(&header.parts), 4);

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

fs::path appendExtension(const fs::path& path, const std::string& extension)
{
	return path.parent_path() / (path.filename().generic_string() + "." + extension);
}

Material createMaterial(fl::Engine& engine, const fs::path& filePath)
{
	const auto absolutePath = Asset::absolute(appendExtension(filePath, "filamat"));

	auto stream = std::ifstream{absolutePath, std::ios_base::in | std::ios_base::binary};
	stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	const auto iterator = std::istreambuf_iterator<char>{stream};
	const auto data = std::vector<char>{iterator, {}};

	auto material = fl::Material::Builder().package(&data[0], data.size()).build(engine);

	return Material{engine, material};
}

Texture createTexture(fl::Engine& engine, const fs::path& filePath)
{
	const auto path = Asset::absolute(filePath);

	int width, height, n;
	const auto data = stbi_load(path.generic_string().c_str(), &width, &height, &n, 4);

	auto buffer =
		fl::Texture::PixelBufferDescriptor{data,
										   size_t(width) * height * 4,
										   fl::Texture::Format::RGBA,
										   fl::Texture::Type::UBYTE,
										   reinterpret_cast<fl::Texture::PixelBufferDescriptor::Callback>(&stbi_image_free)};

	auto texture = fl::Texture::Builder()
							 .width(uint32_t(width))
							 .height(uint32_t(height))
							 .levels(1)
							 .sampler(fl::Texture::Sampler::SAMPLER_2D)
							 .format(fl::Texture::InternalFormat::RGBA8)
							 .build(engine);

	texture->setImage(engine, 0, std::move(buffer));

	return Texture{engine, texture};
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
						 .attribute(filament::POSITION,
									0,
									fl::VertexBuffer::AttributeType::HALF4,
									header.offsetPosition,
									static_cast<uint8_t>(header.stridePosition))
						 .attribute(filament::TANGENTS,
									0,
									fl::VertexBuffer::AttributeType::SHORT4,
									header.offsetTangents,
									static_cast<uint8_t>(header.strideTangents))
						 .attribute(filament::COLOR,
									0,
									fl::VertexBuffer::AttributeType::UBYTE4,
									header.offsetColor,
									static_cast<uint8_t>(header.strideColor))
						 .attribute(filament::UV0, 0, uvType, header.offsetUV0, static_cast<uint8_t>(header.strideUV0))
						 .normalized(filament::UV0, uvNormalized);

	if (header.offsetUV1 != std::numeric_limits<uint32_t>::max() && header.strideUV1 != std::numeric_limits<uint32_t>::max())
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
	auto ibBuilder = fl::IndexBuffer::Builder()
						 .indexCount(header.indexCount)
						 .bufferType(header.indexType ? fl::IndexBuffer::IndexType::USHORT : fl::IndexBuffer::IndexType::UINT);

	auto ib = IndexBuffer{engine, ibBuilder.build(engine)};

	auto ibBufferDescriptor = fl::IndexBuffer::BufferDescriptor(&indices[0], header.indexSize);
	ib->setBuffer(engine, std::move(ibBufferDescriptor));

	fl::Fence::waitAndDestroy(engine.createFence());

	return ib;
}

Mesh createMesh(fl::Engine& engine, const std::filesystem::path& path)
{
	const auto absolutePath = Asset::absolute(appendExtension(path, "filamesh"));

	auto stream = std::ifstream{absolutePath, std::ios_base::in | std::ios_base::binary};
	stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	FilameshFileHeader header;
	stream >> header;

	auto vertices = std::vector<char>(header.vertexSize);
	stream.read(&vertices[0], header.vertexSize);
	VertexBuffer vb = createVertexBuffer(engine, header, vertices);

	auto indices = std::vector<char>(header.indexSize);
	stream.read(&indices[0], header.indexSize);
	IndexBuffer ib = createIndexBuffer(engine, header, indices);

	auto mesh = Mesh{engine, std::move(vb), std::move(ib), header.aabb, header.parts};
	for (size_t i = 0; i < header.parts; i++)
	{
		stream >> mesh[i];
	}

	uint32_t materialCount;
	stream.read(reinterpret_cast<char*>(&materialCount), 4);

	for (size_t i = 0; i < materialCount; i++)
	{
		uint32_t nameLength;
		stream.read(reinterpret_cast<char*>(&nameLength), 4);
		std::getline(stream, mesh[i].materialName, '\0');
	}

	fl::Fence::waitAndDestroy(engine.createFence());

	return mesh;
}

} // namespace spatial
