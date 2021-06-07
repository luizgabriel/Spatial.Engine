#pragma once

#include <array>
#include <filament/Box.h>
#include <string>

#include <spatial/ecs/Registry.h>

namespace spatial::ecs
{

struct Mesh
{
	Entity vertexBufferId;
	Entity indexBufferId;

	filament::Box boundingBox;

	struct Geometry {
		std::uint32_t offset;
		std::uint32_t count;
	};

	std::array<Geometry, 32> geometries;
	std::uint8_t geometriesCount;
};

struct MeshRenderer
{
	bool castShadows;
	bool receiveShadows;

	std::array<Entity, 32> materials;
	std::uint8_t materialsCount;
};

}