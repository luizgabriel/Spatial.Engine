#include <spatial/resources/MeshComponents.h>

namespace spatial
{

ecs::VertexData createVertexData(const FilameshFile& filamesh)
{
	constexpr uint32_t FLAG_SNORM16_UV = 0x2;
	constexpr uint32_t FLAG_VERTEX_ATTR_INTER = 0x1;

	auto uvType = ecs::VertexAttributeType::Half2;
	if (filamesh.header.flags & FLAG_SNORM16_UV)
		uvType = ecs::VertexAttributeType::Short2;

	auto layoutMode = ecs::VertexLayoutMode::Separate;
	if (filamesh.header.flags & FLAG_VERTEX_ATTR_INTER)
		layoutMode = ecs::VertexLayoutMode::Interleaved;

	bool uvNormalized = filamesh.header.flags & FLAG_SNORM16_UV;

	auto layout =
		ecs::VertexLayout{layoutMode,
						  {
							  ecs::VertexDescription{ecs::VertexAttribute::Position, ecs::VertexAttributeType::Half4},
							  {ecs::VertexAttribute::Tangents, ecs::VertexAttributeType::Short4, true},
							  {ecs::VertexAttribute::Color, ecs::VertexAttributeType::UnsignedByte4, true},
							  {ecs::VertexAttribute::UV0, uvType, uvNormalized},
						  }};

	return ecs::VertexData::create(filamesh.vertexData, std::move(layout));
}

ecs::IndexData createIndexData(const FilameshFile& filamesh)
{
	return ecs::IndexData{filamesh.indexData,
						  filamesh.header.indexType ? ecs::IndexType::UnsignedShort : ecs::IndexType::UnsignedInt};
}

}