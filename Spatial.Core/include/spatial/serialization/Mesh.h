#pragma once

#include <cereal/cereal.hpp>
#include <fmt/format.h>
#include <spatial/ecs/Mesh.h>
#include <spatial/serialization/BoundingBox.h>

namespace cereal
{

template <typename Archive>
void serialize(Archive& ar, spatial::ecs::Mesh& mesh)
{
	ar(make_nvp("resourceName", mesh.resourceName));
	ar(make_nvp("receiveShadows", mesh.receiveShadows));
	ar(make_nvp("castShadows", mesh.castShadows));

	for (std::uint8_t i = 0; i < mesh.partsCount; i++)
		ar(make_nvp(fmt::format("material{0}", i), mesh.materials[i]));
}

}
