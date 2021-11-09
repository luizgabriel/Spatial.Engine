#pragma once

#include <array>
#include <filament/Box.h>
#include <filesystem>
#include <spatial/ecs/Registry.h>

namespace spatial::ecs
{

struct Mesh
{
	constexpr static uint8_t MAX_GEOMETRIES = 16;
	constexpr static size_t ALL_PARTS = std::numeric_limits<std::size_t>::max();

	std::filesystem::path resourcePath;

	bool castShadows{false};
	bool receiveShadows{false};
	bool culling{false};

	size_t partsCount{1};
	size_t partsOffset{0};

	Entity defaultMaterial;
	std::array<Entity, Mesh::MAX_GEOMETRIES> materials{};

	Mesh()
		: resourcePath{}, defaultMaterial{NullEntity}
	{
	}

	Mesh(std::filesystem::path resourcePath, Entity defaultMaterial)
		: resourcePath{std::move(resourcePath)}, defaultMaterial{defaultMaterial}
	{
	}
};

} // namespace spatial::ecs