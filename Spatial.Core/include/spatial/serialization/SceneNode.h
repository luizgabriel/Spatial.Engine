#pragma once

#include <cereal/cereal.hpp>
#include <spatial/ecs/EntityName.h>

namespace cereal
{

template <typename Archive>
void serialize(Archive& ar, spatial::ecs::EntityName& node)
{
	ar(make_nvp("name", node.name));
	ar(make_nvp("parent", node.parent));
	ar(make_nvp("next", node.next));
}

} // namespace spatial