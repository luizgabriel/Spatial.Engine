#pragma once

#include <cereal/cereal.hpp>
#include <spatial/ecs/Name.h>

namespace cereal
{

template <typename Archive>
void serialize(Archive& ar, spatial::ecs::Name& node)
{
	ar(make_nvp("name", node.name));
}

} // namespace cereal