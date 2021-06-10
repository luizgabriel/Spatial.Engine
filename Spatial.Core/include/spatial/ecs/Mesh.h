#pragma once

#include <array>
#include <filament/Box.h>
#include <string>

#include <spatial/common/StringHelpers.h>
#include <spatial/ecs/Registry.h>

namespace spatial::ecs
{

struct Mesh
{
	constexpr static std::uint8_t MAX_GEOMETRIES = 32;

	std::string resourceName;

	bool castShadows{true};
	bool receiveShadows{true};

	std::size_t partsCount{1};
	std::size_t partsOffset{0};
	std::array<Entity, Mesh::MAX_GEOMETRIES> materials{};
};

}