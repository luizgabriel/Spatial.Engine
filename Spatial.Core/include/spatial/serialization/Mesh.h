#pragma once

#include <cereal/cereal.hpp>
#include <spatial/ecs/Mesh.h>
#include <spatial/serialization/BoundingBox.h>
#include <spatial/serialization/Resource.h>

namespace cereal
{

template <typename Archive>
void serialize(Archive& ar, spatial::ecs::MeshInstance& mesh)
{
	ar(make_nvp("meshSource", mesh.meshSource));
	ar(make_nvp("defaultMaterial", mesh.defaultMaterial));
	ar(make_nvp("receiveShadows", mesh.receiveShadows));
	ar(make_nvp("castShadows", mesh.castShadows));
	ar(make_nvp("culling", mesh.culling));
	ar(make_nvp("priority", mesh.priority));
	ar(make_nvp("partsCount", mesh.slice.count));
	ar(make_nvp("partsOffset", mesh.slice.offset));
}

template <typename Archive>
void serialize(Archive& ar, spatial::ecs::MeshMaterial& meshMaterial)
{
	ar(make_nvp("primitiveIndex", meshMaterial.primitiveIndex));
	ar(make_nvp("materialEntity", meshMaterial.materialInstanceEntity));
}

} // namespace cereal
