#pragma once

#include "Materials.h"
#include "EditorCamera.h"
#include "Tags.h"

#include <cereal/cereal.hpp>
#include <magic_enum.hpp>
#include <spatial/serialization/Archives.h>
#include <spatial/serialization/Math.h>
#include <spatial/serialization/Registry.h>
#include <spatial/resources/ResourceLoader.h>

namespace spatial::editor
{

tl::expected<ecs::Registry, ResourceError> parseRegistry(std::istream&& istream);

} // namespace spatial

namespace cereal
{

template <typename Archive>
void serialize(Archive& ar, spatial::editor::EditorCamera& camera)
{
	ar(cereal::make_nvp("velocity", camera.velocity));
	ar(cereal::make_nvp("sensitivity", camera.sensitivity));
}

template <typename Archive>
void serialize(Archive& ar, spatial::editor::DefaultMaterial& material)
{
	ar(cereal::make_nvp("baseColor", material.baseColor));
	ar(cereal::make_nvp("roughness", material.roughness));
	ar(cereal::make_nvp("metallic", material.metallic));
	ar(cereal::make_nvp("reflectance", material.reflectance));
}

} // namespace cereal