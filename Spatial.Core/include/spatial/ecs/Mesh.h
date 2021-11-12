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

	//TODO: Use a resource UUID instead of the direct path
	std::filesystem::path resourcePath;

	bool castShadows{false};
	bool receiveShadows{false};
	bool culling{false};

	size_t partsCount{0};
	size_t partsOffset{0};

	Entity defaultMaterial{ecs::NullEntity};

	explicit Mesh(std::filesystem::path resourcePath = {})
		: resourcePath{std::move(resourcePath)}
	{
	}
};

} // namespace spatial::ecs