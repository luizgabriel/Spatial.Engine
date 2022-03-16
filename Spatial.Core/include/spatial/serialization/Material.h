#pragma once

#include <cereal/cereal.hpp>
#include <spatial/ecs/Material.h>
#include <spatial/serialization/Resource.h>

namespace cereal
{

template <typename Archive>
void serialize(Archive& ar, spatial::ecs::PrecompiledMaterial& material)
{
	ar(make_nvp("resource", material.resource));
}

} // namespace cereal
