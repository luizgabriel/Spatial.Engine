#pragma once

#include <spatial/ecs/Registry.h>
#include <spatial/resources/Resource.h>

namespace spatial::ecs
{

namespace tags
{

struct IsMeshDirty
{
};

struct IsMeshLoaded
{
};

} // namespace tags

struct MeshMaterial
{
	size_t primitiveIndex;
	Entity materialEntity;

	constexpr explicit MeshMaterial(size_t primitiveIndex = 0, Entity materialEntity = ecs::NullEntity)
		: primitiveIndex(primitiveIndex), materialEntity(materialEntity)
	{
	}
};

struct Mesh
{
	Resource<FilaMesh> resource;

	explicit Mesh() = default;
	explicit Mesh(const Resource<FilaMesh>& resource) : resource{resource}
	{
	}

	static Entity findByResource(const Registry& registry, const std::filesystem::path& resource);

	static Entity findOrCreateResource(Registry& registry, const std::filesystem::path& resource);
};


struct MeshInstance
{
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

	bool castShadows;
	bool receiveShadows;
	bool culling;
	uint8_t priority;

	Slice slice;

	MeshInstance() = default;

	static void addMaterial(Registry& registry, Entity& meshEntity, Entity materialEntity);

	static void addMaterial(Registry& registry, Entity& meshEntity, Entity materialEntity, size_t primitiveIndex);
};

} // namespace spatial::ecs