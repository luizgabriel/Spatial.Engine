#pragma once

#include "EditorCamera.h"
#include "DefaultMaterial.h"

#include <cereal/cereal.hpp>
#include <magic_enum.hpp>
#include <spatial/serialization/Math.h>

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

}