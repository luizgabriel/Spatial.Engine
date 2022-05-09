#pragma once

#include <cereal/cereal.hpp>
#include <spatial/ecs/Texture.h>

namespace cereal
{

template <typename Archive>
void serialize(Archive& ar, spatial::ecs::RuntimeTexture& mesh)
{
	ar(make_nvp("pixels", mesh.pixels));
	ar(make_nvp("width", mesh.width));
}

} // namespace cereal