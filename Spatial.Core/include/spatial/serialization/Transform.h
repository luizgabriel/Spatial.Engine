#pragma once

#include <cereal/cereal.hpp>
#include <spatial/ecs/Transform.h>
#include <spatial/serialization/Math.h>

namespace cereal
{

template <typename Archive>
void serialize(Archive& ar, spatial::ecs::Transform& transform)
{
	ar(make_nvp("position", transform.position));
	ar(make_nvp("rotation", transform.rotation));
	ar(make_nvp("scale", transform.scale));
}

} // namespace cereal