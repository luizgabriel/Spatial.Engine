#pragma once

#include <cereal/cereal.hpp>
#include <spatial/ecs/EntityName.h>

namespace cereal
{

template <typename Archive>
void serialize(Archive& ar, spatial::ecs::EntityName& node)
{
	ar(make_nvp("name", node.name));
}

template <typename Archive>
void save(Archive& ar, const spatial::ecs::UUID& id)
{
	ar(make_nvp("uuid", id.toString()));
}

template <typename Archive>
void load(Archive& ar, spatial::ecs::UUID& id)
{
	std::string uuid;
	ar(make_nvp("uuid", uuid));
	id = spatial::ecs::UUID::fromString(uuid);
}


} // namespace spatial