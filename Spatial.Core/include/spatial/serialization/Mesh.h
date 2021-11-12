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
	ar(make_nvp("defaultMaterial", mesh.defaultMaterial));
}

}
