#pragma once

#include <spatial/ecs/Tags.h>

namespace cereal
{

template <typename Archive>
void serialize(Archive& ar, spatial::ecs::tags::IsRenderable&)
{
	auto tag = "is-renderable";
	ar(CEREAL_NVP(tag));
}

template <typename Archive>
void serialize(Archive& ar, spatial::ecs::tags::IsMeshMaterial&)
{
	auto tag = "is-material";
	ar(CEREAL_NVP(tag));
}

}