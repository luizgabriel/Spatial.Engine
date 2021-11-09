#pragma once

#include <cereal/cereal.hpp>
#include <fmt/format.h>
#include <spatial/ecs/Mesh.h>
#include <spatial/serialization/BoundingBox.h>
#include <spatial/serialization/FileSystem.h>

namespace cereal
{

template <typename Archive>
void serialize(Archive& ar, spatial::ecs::Mesh& mesh)
{
	ar(make_nvp("resourcePath", mesh.resourcePath));
	ar(make_nvp("receiveShadows", mesh.receiveShadows));
	ar(make_nvp("castShadows", mesh.castShadows));
	ar(make_nvp("partsCount", mesh.partsCount));
	ar(make_nvp("partsOffset", mesh.partsOffset));

	using parts_t = decltype(mesh.partsOffset);
	for (parts_t i = 0; i < mesh.partsCount; i++) {
		ar(make_nvp(fmt::format("material{0}", i), mesh.materials[i]));
	}
}

}
