#pragma once

#include <cereal/cereal.hpp>
#include <fmt/format.h>
#include <spatial/ecs/Mesh.h>
#include <spatial/serialization/BoundingBox.h>

namespace cereal
{

template <typename Archive>
void serialize(Archive& ar, spatial::ecs::Mesh::Geometry& geometry)
{
	ar(make_nvp("count", geometry.count));
	ar(make_nvp("offset", geometry.offset));
}

template <typename Archive>
void serialize(Archive& ar, spatial::ecs::Mesh& mesh)
{
	ar(make_nvp("vertexBufferId", mesh.vertexBufferId));
	ar(make_nvp("indexBufferId", mesh.indexBufferId));

	ar(make_nvp("boundingBox", mesh.boundingBox));
	ar(make_nvp("geometriesCount", mesh.geometriesCount));

	for (std::uint8_t i = 0; i < mesh.geometriesCount; i++)
		ar(make_nvp(fmt::format("geometry{0}", i), mesh.geometries[i]));
}

template <typename Archive>
void serialize(Archive& ar, spatial::ecs::MeshRenderer& mesh)
{
	ar(make_nvp("receiveShadows", mesh.receiveShadows));
	ar(make_nvp("castShadows", mesh.castShadows));

	ar(make_nvp("materialsCount", mesh.materialsCount));

	for (std::uint8_t i = 0; i < mesh.materialsCount; i++)
		ar(make_nvp(fmt::format("material{0}", i), mesh.materials[i]));
}

}
