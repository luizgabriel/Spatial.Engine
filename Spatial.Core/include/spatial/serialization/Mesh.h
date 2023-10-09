#pragma once

#include <cereal/cereal.hpp>
#include <cereal/types/array.hpp>
#include <spatial/ecs/Mesh.h>
#include <spatial/serialization/BoundingBox.h>
#include <spatial/serialization/Resource.h>

namespace cereal
{

template <typename Archive>
void serialize(Archive& ar, spatial::ecs::MeshInstance& mesh)
{
	ar(make_nvp("meshSource", mesh.meshSource));
	ar(make_nvp("defaultMaterialInstance", mesh.defaultMaterialInstance));
	ar(make_nvp("receiveShadows", mesh.receiveShadows));
	ar(make_nvp("castShadows", mesh.castShadows));
	ar(make_nvp("culling", mesh.culling));
	ar(make_nvp("priority", mesh.priority));
}

template <typename Archive>
void serialize(Archive& ar, spatial::ecs::VertexDescription& description)
{
	ar(make_nvp("attribute", description.attribute));
	ar(make_nvp("type", description.type));
	ar(make_nvp("isNormalized", description.isNormalized));
}

template <typename Archive>
void serialize(Archive& ar, spatial::ecs::VertexLayout& layout)
{
	ar(make_nvp("mode", layout.mode));
	ar(make_nvp("description", layout.description));
}

template <typename Archive>
void serialize(Archive& ar, spatial::ecs::VertexData& mesh)
{
	ar(make_nvp("minCount", mesh.minCount));
	ar(make_nvp("data", mesh.data));
	ar(make_nvp("layout", mesh.layout));
}

template <typename Archive>
void serialize(Archive& ar, spatial::ecs::IndexData& mesh)
{
	ar(make_nvp("minCount", mesh.minCount));
	ar(make_nvp("data", mesh.data));
	ar(make_nvp("type", mesh.type));
}

template <typename Archive>
void serialize(Archive& ar, spatial::ecs::MeshPart& part)
{
	ar(make_nvp("minIndex", part.minIndex));
	ar(make_nvp("indexCount", part.indexCount));
}

template <typename Archive>
void serialize(Archive& ar, spatial::ecs::MeshPrimitive& primitive)
{
	ar(make_nvp("materialInstance", primitive.materialInstance));
	ar(make_nvp("primitiveIndex", primitive.primitiveIndex));
	ar(make_nvp("blendOrder", primitive.blendOrder));
}

template <typename Archive>
void serialize(Archive& ar, spatial::ecs::Mesh& mesh)
{
	ar(make_nvp("vertexData", mesh.vertexData));
	ar(make_nvp("indexData", mesh.indexData));
	ar(make_nvp("boundingBox", mesh.boundingBox));
}

} // namespace cereal
