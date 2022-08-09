#include <filament/Fence.h>
#include <spatial/graphics/MeshResources.h>

namespace spatial::graphics
{

filament::VertexBuffer::Builder createVertexBufferBuilder(const FilameshFileHeader& header)
{
	const uint32_t FLAG_SNORM16_UV = 0x2;
	auto uvType = filament::VertexBuffer::AttributeType::HALF2;
	if (header.flags & FLAG_SNORM16_UV)
	{
		uvType = filament::VertexBuffer::AttributeType::SHORT2;
	}

	bool uvNormalized = header.flags & FLAG_SNORM16_UV;

	auto vbBuilder = filament::VertexBuffer::Builder()
						 .vertexCount(header.vertexCount)
						 .bufferCount(1)
						 .normalized(filament::TANGENTS)
						 .normalized(filament::COLOR)
						 .attribute(filament::POSITION, 0, filament::VertexBuffer::AttributeType::HALF4,
									header.offsetPosition, static_cast<uint8_t>(header.stridePosition))
						 .attribute(filament::TANGENTS, 0, filament::VertexBuffer::AttributeType::SHORT4,
									header.offsetTangents, static_cast<uint8_t>(header.strideTangents))
						 .attribute(filament::COLOR, 0, filament::VertexBuffer::AttributeType::UBYTE4,
									header.offsetColor, static_cast<uint8_t>(header.strideColor))
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

VertexBuffer createVertexBuffer(filament::Engine& engine, const FilameshFile& filamesh)
{
	auto vb = VertexBuffer{engine, createVertexBufferBuilder(filamesh.header).build(engine)};
	vb->setBufferAt(engine, 0, createVertexBufferDescriptor(filamesh));

	filament::Fence::waitAndDestroy(engine.createFence());

	return vb;
}

filament::IndexBuffer::Builder createIndexBufferBuilder(const FilameshFileHeader& header)
{
	return filament::IndexBuffer::Builder()
		.indexCount(header.indexCount)
		.bufferType(header.indexType ? filament::IndexBuffer::IndexType::USHORT
									 : filament::IndexBuffer::IndexType::UINT);
}

filament::IndexBuffer::BufferDescriptor createIndexBufferDescriptor(const FilameshFile& filamesh)
{
	return {filamesh.indexData.data(), filamesh.header.indexSize};
}

IndexBuffer createIndexBuffer(filament::Engine& engine, const FilameshFile& filamesh)
{
	auto ib = IndexBuffer{engine, createIndexBufferBuilder(filamesh.header).build(engine)};
	ib->setBuffer(engine, createIndexBufferDescriptor(filamesh));

	filament::Fence::waitAndDestroy(engine.createFence());

	return ib;
}

MeshGeometries createMeshGeometries(const FilameshFile& filamesh)
{
	auto geometries = MeshGeometries(filamesh.parts.size());

	for (size_t i = 0; i < filamesh.parts.size(); i++)
	{
		const auto& part = filamesh.parts[i];
		geometries.at(i) = MeshGeometry{part.offset, part.count};
	}

	return geometries;
}

} // namespace spatial::graphics