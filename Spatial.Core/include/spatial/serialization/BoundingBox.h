#pragma once

#include <cereal/cereal.hpp>
#include <filament/Box.h>
#include <spatial/serialization/Math.h>

namespace cereal
{

template <typename Archive>
void serialize(Archive& ar, filament::Box& box)
{
	ar(cereal::make_nvp("center", box.center));
	ar(cereal::make_nvp("halfExtent", box.halfExtent));
}

} // namespace cereal