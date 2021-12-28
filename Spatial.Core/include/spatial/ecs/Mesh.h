#pragma once

#include <array>
#include <filament/Box.h>
#include <filesystem>
#include <spatial/common/StringHelpers.h>
#include <spatial/ecs/Registry.h>
#include <spatial/ecs/Resource.h>

namespace spatial::ecs
{

struct Mesh
{
	ecs::Resource<ecs::ResourceType::FilaMesh> meshResource{};

	bool castShadows{false};
	bool receiveShadows{false};
	bool culling{false};

	size_t partsCount{};
	size_t partsOffset{};
	uint8_t priority{};
};

struct MeshMaterial
{
	uint32_t primitiveIndex;
	Entity materialEntity;

	constexpr MeshMaterial(uint32_t primitiveIndex = 0, Entity materialEntity = ecs::NullEntity)
		: primitiveIndex(primitiveIndex), materialEntity(materialEntity)
	{
	}
};

} // namespace spatial::ecs