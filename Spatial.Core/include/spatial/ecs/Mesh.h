#pragma once

#include <spatial/ecs/Registry.h>
#include <spatial/ecs/EntityHandle.h>
#include <spatial/resources/Resource.h>

namespace spatial::ecs
{

namespace tags
{

struct IsMesh
{
	constexpr static auto typeName = "tag_is_mesh";
};

struct IsMeshInstanceDirty
{
};

struct IsMeshLoaded
{
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

struct Mesh
{
	constexpr static auto typeName = "mesh";

	Resource<FilaMesh> resource;

	Mesh() = default;

	explicit Mesh(const Resource<FilaMesh>& resource) : resource{resource}
	{
	}

	static EntityConstHandle find(const Registry& registry, const std::filesystem::path& resource);

	static EntityHandle findOrCreate(Registry& registry, const std::filesystem::path& resource);
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

	bool castShadows{false};
	bool receiveShadows{false};
	bool culling{true};
	uint8_t priority{0};

	Slice slice{};

	MeshInstance() = default;

	static void addMaterial(Registry& registry, Entity meshEntity, Entity materialEntity);

	static void addMaterial(Registry& registry, Entity meshEntity, Entity materialEntity, size_t primitiveIndex);
};

} // namespace spatial::ecs