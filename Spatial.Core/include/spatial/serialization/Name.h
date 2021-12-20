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

template <typename Archive>
void save(Archive& ar, const spatial::ecs::UUID& id)
{
	ar(id.toString());
}

template <typename Archive>
void load(Archive& ar, spatial::ecs::UUID& id)
{
	std::string uuid;
	ar(uuid);
	if (uuid.size() > 0)
		id = spatial::ecs::UUID::fromString(uuid);
}

} // namespace cereal