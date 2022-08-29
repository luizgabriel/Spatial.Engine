#pragma once

#include <spatial/ecs/Registry.h>
#include <spatial/common/AxisAlignedBoundingBox.h>
#include <vector>
#include <array>

namespace spatial::ecs
{

namespace tags
{

struct IsMesh
{
	constexpr static auto typeName = "tag_is_mesh";
};

struct ShouldRecreateRenderable
{
};

struct ShouldRecreateMesh
{
};

} // namespace tags

enum class VertexAttribute
{
	Position,
	Color,
	UV0,
	Tangents
};

enum class VertexAttributeType : uint16_t
{
	None = 0,
	// 		      (Order << 8) | (Dimension * DataSize)
	Float2 			= (1 << 8) | (4 * 2),
	Float3 			= (2 << 8) | (4 * 3),
	UnsignedByte3 	= (3 << 8) | (3 * 1),
	UnsignedByte4 	= (4 << 8) | (4 * 1),
	Half2 			= (5 << 8) | (2 * 2),
	Half4 			= (6 << 8) | (4 * 2),
	Short2 			= (7 << 8) | (2 * 2),
	Short3 			= (7 << 8) | (3 * 2),
	Short4 			= (7 << 8) | (4 * 2),
};

constexpr uint8_t getVertexAttributeSize(VertexAttributeType type)
{
	return static_cast<uint8_t>(0x00FF & static_cast<uint16_t>(type));
}

static_assert(getVertexAttributeSize(VertexAttributeType::Float2) == 8, "Float2 should be 8 bytes");
static_assert(getVertexAttributeSize(VertexAttributeType::Float3) == 12, "Float3 should be 12 bytes");

struct VertexDescription
{
	VertexAttribute attribute{VertexAttribute::Position};
	VertexAttributeType type{VertexAttributeType::None};
	bool isNormalized{false};

	constexpr uint8_t getTypeSizeInBytes() const
	{
		return getVertexAttributeSize(type);
	}

	constexpr bool empty() const
	{
		return getTypeSizeInBytes() == 0;
	}
};

enum class VertexLayoutMode
{
	Interleaved,
	Separate
};

struct VertexLayout
{
	VertexLayoutMode mode{VertexLayoutMode::Interleaved};
	std::array<VertexDescription, 4> description{};
};

constexpr uint8_t calculateInterleavedBytesStride(const VertexLayout& layout)
{
	uint8_t total = 0;
	for (const auto& attr : layout.description)
		total += attr.getTypeSizeInBytes();

	return total;
}

struct VertexData
{
	std::vector<uint8_t> data{};
	VertexLayout layout{};
	size_t minCount{0};

	constexpr VertexData() = default;

	VertexData(std::vector<uint8_t> data, VertexLayout layout)
		: data{std::move(data)}, layout{std::move(layout)}
	{
	}

	constexpr size_t getDesiredBufferSize() const
	{
		return std::max(minCount, data.size());
	}

	template <typename T>
	static VertexData create(const std::vector<T>& data, VertexLayout layout)
	{
		const auto* beginData = reinterpret_cast<const uint8_t*>(data.data());
		const auto* endData = beginData + sizeof(T) * data.size();
		auto buffer = std::vector<uint8_t>(beginData, endData);

		return {std::move(buffer), std::move(layout)};
	}
};

enum class IndexType : uint8_t
{
	None = 0,
	UnsignedShort = sizeof(uint16_t),
	UnsignedInt = sizeof(uint32_t),
};

struct IndexData
{
	std::vector<uint8_t> data{};
	IndexType type{IndexType::UnsignedShort};
	size_t minCount{0};

	constexpr IndexData() = default;

	IndexData(std::vector<uint8_t> data, IndexType type) : data{std::move(data)}, type{type}
	{
	}

	constexpr size_t getDesiredBufferSize() const
	{
		return std::max(minCount, data.size());
	}

	template <typename T>
	static IndexData create(const std::vector<T>& data)
	{
		static_assert(std::is_same_v<T, uint16_t> || std::is_same_v<T, uint32_t>, "Unsupported index data type");

		const auto* beginData = reinterpret_cast<const uint8_t*>(data.data());
		const auto* endData = beginData + sizeof(T) * data.size();
		auto buffer = std::vector<uint8_t>(beginData, endData);

		if constexpr (std::is_same_v<T, uint16_t>)
			return {std::move(buffer), IndexType::UnsignedShort};

		return {std::move(buffer), IndexType::UnsignedInt};
	}
};

struct MeshPart
{
	constexpr static auto typeName = "mesh_part";

	uint32_t minIndex{0};
	uint32_t indexCount{0};
};

struct Mesh
{
	constexpr static auto typeName = "mesh";

	VertexData vertexData{};
	IndexData indexData{};
	math::AxisAlignedBoundingBox boundingBox{};

	static Entity addPart(Registry& registry, Entity meshEntity, MeshPart part);
	static std::vector<Entity> getParts(const Registry& registry, Entity meshEntity);
};

struct MeshInstance
{
	constexpr static auto typeName = "mesh_instance";

	Entity meshSource{ecs::NullEntity};
	Entity defaultMaterialInstance{ecs::NullEntity};

	bool castShadows{false};
	bool receiveShadows{false};
	bool culling{true};
	uint8_t priority{0};

	size_t partsOffset{0};
	size_t partsCount{1};

	static size_t getPrimitivesCount(const Registry& registry, Entity meshInstanceEntity);

	static Entity addMaterial(Registry& registry, Entity meshInstanceEntity, Entity materialInstanceEntity, size_t primitiveIndex);

	static Entity addMaterial(Registry& registry, Entity meshInstanceEntity, Entity materialInstanceEntity);

	static std::vector<Entity> getPrimitives(const ecs::Registry& registry, Entity meshInstance);

	static size_t destroyPrimitives(Registry& registry, Entity meshInstanceEntity);
};

struct MeshPrimitive
{
	constexpr static auto typeName = "mesh_primitive";

	size_t primitiveIndex{0};
	Entity materialInstance{ecs::NullEntity};
	uint16_t blendOrder{0};

	static void changeMeshInstance(Registry& registry, Entity meshPrimitiveEntity, Entity meshInstanceEntity);
};

} // namespace spatial::ecs