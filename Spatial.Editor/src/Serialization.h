#pragma once

#include "EditorCamera.h"
#include "Materials.h"
#include "Tags.h"

#include <cereal/cereal.hpp>
#include <spatial/ecs/Registry.h>
#include <spatial/serialization/Math.h>
#include <spatial/serialization/Resource.h>

namespace spatial::editor
{

ecs::Registry parseRegistry(std::istream& istream);
void writeRegistry(const ecs::Registry& registry, std::ostream& ostream);

} // namespace spatial::editor

namespace cereal
{

template <typename Archive>
void serialize(Archive& ar, spatial::editor::EditorCamera& camera)
{
	ar(make_nvp("velocity", camera.velocity));
	ar(make_nvp("sensitivity", camera.sensitivity));
}

template <typename Archive>
void serialize(Archive& ar, spatial::editor::ColorMaterial& material)
{
	ar(make_nvp("baseColor", material.baseColor));
	ar(make_nvp("roughness", material.roughness));
	ar(make_nvp("metallic", material.metallic));
	ar(make_nvp("reflectance", material.reflectance));
}

template <typename Archive>
void serialize(Archive& ar, spatial::editor::SkyBoxMaterial& material)
{
	ar(make_nvp("skybox", material.skybox));
	ar(make_nvp("showSun", material.showSun));
	ar(make_nvp("color", material.color));
}

template <typename Archive>
void serialize(Archive& ar, spatial::editor::GridMaterial& material)
{
	ar(make_nvp("color", material.color));
	ar(make_nvp("scale", material.scale));
	ar(make_nvp("thickness", material.thickness));
}

template <typename Archive>
void serialize(Archive& ar, spatial::editor::StandardOpaqueMaterial& material)
{
	ar(make_nvp("baseColor", material.baseColor));
	ar(make_nvp("albedo", material.albedo));
	ar(make_nvp("tiling", material.tiling));
	ar(make_nvp("offset", material.offset));
	ar(make_nvp("metallic", material.metallic));
	ar(make_nvp("metallicMap", material.metallicMap));
	ar(make_nvp("roughness", material.roughness));
	ar(make_nvp("roughnessMap", material.roughnessMap));
	ar(make_nvp("reflectance", material.reflectance));
	ar(make_nvp("reflectanceMap", material.reflectanceMap));
	ar(make_nvp("ambientOcclusionMap", material.ambientOcclusionMap));
	ar(make_nvp("normalMap", material.normalMap));
	ar(make_nvp("bentNormalMap", material.bentNormalMap));
	ar(make_nvp("emissive", material.emissive));
	ar(make_nvp("emissiveMap", material.emissiveMap));
	ar(make_nvp("height", material.height));
	ar(make_nvp("heightMap", material.heightMap));
}

} // namespace cereal