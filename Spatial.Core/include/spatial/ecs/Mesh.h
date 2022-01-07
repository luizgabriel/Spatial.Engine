#pragma once

#include <spatial/ecs/Registry.h>
#include <spatial/resources/Resource.h>

namespace spatial::ecs
{

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
	Resource<ResourceType::FilaMesh> meshResource{};

	bool castShadows{false};
	bool receiveShadows{false};
	bool culling{false};

	size_t partsCount{};
	size_t partsOffset{};
	uint8_t priority{};

	static void addMaterial(Registry& registry, Entity& meshEntity, Entity materialEntity);
	static void addMaterial(Registry& registry, Entity& meshEntity, Entity materialEntity, size_t primitiveIndex);
};

} // namespace spatial::ecs