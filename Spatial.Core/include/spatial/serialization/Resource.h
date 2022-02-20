#pragma once

#include <cereal/cereal.hpp>
#include <spatial/resources/Resource.h>

namespace cereal
{

template <typename Archive, spatial::ResourceType type>
void serialize(Archive& ar, spatial::Resource<type>& parent)
{
	ar(make_nvp("relativePath", parent.relativePath));
}

} // namespace cereal