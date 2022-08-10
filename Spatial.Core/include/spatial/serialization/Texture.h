#pragma once

#include <cereal/cereal.hpp>
#include <spatial/ecs/Texture.h>

namespace cereal
{

template <typename Archive>
void serialize(Archive& ar, spatial::ecs::RuntimeTexture& texture)
{
	ar(make_nvp("pixels", texture.pixels));
	ar(make_nvp("width", texture.width));
}

template <typename Archive>
void serialize(Archive& ar, spatial::ecs::AttachmentTexture& attachment)
{
	ar(make_nvp("type", attachment.type));
	ar(make_nvp("size", attachment.size));
}

} // namespace cereal