#pragma once

#include <array>
#include <filament/Box.h>
#include <filesystem>
#include <spatial/common/StringHelpers.h>
#include <spatial/ecs/Registry.h>
#include <spatial/resources/Resource.h>

namespace spatial::ecs
{

struct Mesh
{
	Resource<ResourceType::FilaMesh> meshResource{};

	bool castShadows{false};
	bool receiveShadows{false};
	bool culling{false};

	size_t partsCount{};
	size_t partsOffset{};
	uint8_t priority{};
};

struct MeshMaterial
{
	size_t primitiveIndex;
	Entity materialEntity;

	constexpr explicit MeshMaterial(size_t primitiveIndex = 0, Entity materialEntity = ecs::NullEntity)
		: primitiveIndex(primitiveIndex), materialEntity(materialEntity)
	{
	}
};

} // namespace spatial::ecs