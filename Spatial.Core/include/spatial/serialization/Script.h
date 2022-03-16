#pragma once

#include <cereal/cereal.hpp>
#include <spatial/ecs/Mesh.h>
#include <spatial/serialization/BoundingBox.h>
#include <spatial/serialization/Resource.h>
#include <spatial/ecs/Script.h>

namespace cereal
{

template <typename Archive>
void serialize(Archive& ar, spatial::ecs::Script& script)
{
	ar(make_nvp("resource", script.resource));
}

} // namespace cereal
