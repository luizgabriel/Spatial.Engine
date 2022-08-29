#pragma once

#include <cereal/cereal.hpp>
#include <spatial/ecs/Material.h>
#include <spatial/serialization/Math.h>

namespace cereal {

template <typename Archive>
void serialize(Archive& ar, spatial::ecs::MaterialInstance::Scissor& scissor)
{
	ar(cereal::make_nvp("left", scissor.left));
	ar(cereal::make_nvp("bottom", scissor.bottom));
	ar(cereal::make_nvp("width", scissor.width));
	ar(cereal::make_nvp("height", scissor.height));
}

template <typename Archive>
void serialize(Archive& ar, spatial::ecs::MaterialInstance& light)
{
	ar(cereal::make_nvp("scissor", light.scissor));
}

} // namespace cereal