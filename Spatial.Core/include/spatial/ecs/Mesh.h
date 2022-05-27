#pragma once

#include <spatial/ecs/Registry.h>
#include <spatial/common/Math.h>
#include <vector>

namespace spatial::ecs
{

namespace tags
{

struct IsMesh
{
	constexpr static auto typeName = "tag_is_mesh";
};

} // namespace tags

struct RuntimeMesh
{
	constexpr static auto typeName = "runtime_mesh";

	std::vector<math::vec3> vertexData{};
	std::vector<uint16_t> indexData{};
};

struct MeshMaterial
{
	constexpr static auto typeName = "mesh_material";

	size_t primitiveIndex{0};
	Entity materialInstanceEntity{ecs::NullEntity};

	MeshMaterial() = default;
};

struct MeshInstance
{
	constexpr static auto typeName = "mesh_instance";

	struct Slice
	{
		size_t offset{0};
		size_t count{0};
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