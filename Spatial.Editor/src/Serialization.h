#pragma once

#include "EditorCamera.h"
#include "Materials.h"
#include "Tags.h"

#include <cereal/cereal.hpp>
#include <spatial/ecs/Registry.h>
#include <spatial/resources/ResourceError.h>
#include <spatial/serialization/Math.h>
#include <tl/expected.hpp>

namespace spatial::editor
{

tl::expected<ecs::Registry, ResourceError> parseRegistry(std::istream&& istream);
void writeRegistry(const ecs::Registry& registry, std::ostream&& ostream);

} // namespace spatial::editor

namespace cereal
{

template <typename Archive>
void serialize(Archive& ar, spatial::editor::EditorCamera& camera)
{
	ar(cereal::make_nvp("velocity", camera.velocity));
	ar(cereal::make_nvp("sensitivity", camera.sensitivity));
}

template <typename Archive>
void serialize(Archive& ar, spatial::editor::ColorMaterial& material)
{
	ar(cereal::make_nvp("baseColor", material.baseColor));
	ar(cereal::make_nvp("roughness", material.roughness));
	ar(cereal::make_nvp("metallic", material.metallic));
	ar(cereal::make_nvp("reflectance", material.reflectance));
}

template <typename Archive>
void serialize(Archive& ar, spatial::editor::SkyBoxMaterial& material)
{
	ar(cereal::make_nvp("skybox", material.skybox.relativePath));
	ar(cereal::make_nvp("showSun", material.showSun));
	ar(cereal::make_nvp("color", material.color));
}

template <typename Archive>
void serialize(Archive& ar, spatial::editor::GridMaterial& material)
{
	ar(cereal::make_nvp("color", material.color));
	ar(cereal::make_nvp("scale", material.scale));
	ar(cereal::make_nvp("thickness", material.thickness));
}

} // namespace cereal