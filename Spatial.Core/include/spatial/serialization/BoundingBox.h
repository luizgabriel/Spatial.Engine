#pragma once

#include <cereal/cereal.hpp>
#include <spatial/common/AxisAlignedBoundingBox.h>
#include <spatial/serialization/Math.h>

namespace cereal
{

template <typename Archive>
void serialize(Archive& ar, spatial::math::AxisAlignedBoundingBox& box)
{
	ar(cereal::make_nvp("maximumPoint", box.maximumPoint));
	ar(cereal::make_nvp("minimumPoint", box.minimumPoint));
}

} // namespace cereal