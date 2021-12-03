#pragma once

#include <cereal/cereal.hpp>
#include <spatial/ecs/Relation.h>

namespace cereal
{

template <typename Archive>
void serialize(Archive& ar, spatial::ecs::Parent& parent)
{
	ar(make_nvp("childrenCount", parent.childrenCount));
	ar(make_nvp("first", parent.first));
	ar(make_nvp("last", parent.last));
}

template <typename Archive>
void serialize(Archive& ar, spatial::ecs::Child& child)
{
	ar(make_nvp("parent", child.parent));
	ar(make_nvp("next", child.next));
	ar(make_nvp("previous", child.previous));
}

}