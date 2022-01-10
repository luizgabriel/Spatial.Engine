#pragma once

#include <cereal/cereal.hpp>
#include <spatial/ecs/Mesh.h>
#include <spatial/serialization/BoundingBox.h>
#include <spatial/serialization/FileSystem.h>
#include <spatial/serialization/Resource.h>

namespace cereal
{

template <typename Archive>
void serialize(Archive& ar, spatial::ecs::Mesh& mesh)
{
	ar(make_nvp("resourcePath", mesh.meshResource));
	ar(make_nvp("receiveShadows", mesh.receiveShadows));
	ar(make_nvp("castShadows", mesh.castShadows));
	ar(make_nvp("partsCount", mesh.partsCount));
	ar(make_nvp("partsOffset", mesh.partsOffset));
}

template <typename Archive>
void serialize(Archive& ar, spatial::ecs::MeshMaterial& meshMaterial)
{
	ar(make_nvp("primitiveIndex", meshMaterial.primitiveIndex));
	ar(make_nvp("materialEntity", meshMaterial.materialEntity));
}

} // namespace cereal
