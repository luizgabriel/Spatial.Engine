#pragma once

#include <cereal/cereal.hpp>
#include <spatial/ecs/Resource.h>

namespace cereal
{

template <typename Archive>
void serialize(Archive& ar, spatial::ecs::Resource& resource)
{
	ar(make_nvp("relativePath", resource.relativePath));
}

} // namespace cereal