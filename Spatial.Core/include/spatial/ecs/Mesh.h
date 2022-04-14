#pragma once

#include <spatial/ecs/Registry.h>
#include <spatial/resources/ResourcePath.h>

namespace spatial::ecs
{

namespace tags
{

struct IsMesh
{
	constexpr static auto typeName = "tag_is_mesh";
};

} // namespace tags

struct MeshMaterial
{
	constexpr static auto typeName = "mesh_material";

	size_t primitiveIndex;
	Entity materialInstanceEntity;

	constexpr explicit MeshMaterial(size_t primitiveIndex = 0, Entity materialEntity = ecs::NullEntity)
		: primitiveIndex(primitiveIndex), materialInstanceEntity(materialEntity)
	{
	}
};

struct MeshInstance
{
	constexpr static auto typeName = "mesh_instance";

	struct Slice
	{
		size_t offset;
		size_t count;

		Slice() = default;
		constexpr Slice(size_t offset, size_t count) : offset(offset), count(count)
		{
		}
	};

	Entity meshSource{ecs::NullEntity};

	Entity defaultMaterial{ecs::NullEntity};

	bool castShadows{false};
	bool receiveShadows{false};
	bool culling{true};
	uint8_t priority{0};

	Slice slice{};

	MeshInstance() = default;

	static Entity addMaterial(Registry& registry, Entity meshEntity, Entity materialEntity);

	static Entity addMaterial(Registry& registry, Entity meshEntity, Entity materialEntity, size_t primitiveIndex);
};

} // namespace spatial::ecs