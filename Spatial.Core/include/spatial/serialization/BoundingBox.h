#pragma once

#include <cereal/cereal.hpp>
#include <spatial/common/AxisAlignedBoundingBox.h>
#include <spatial/serialization/Math.h>

namespace cereal
{

template <typename Archive>
void serialize(Archive& archive, spatial::math::AxisAlignedBoundingBox& box)
{
	archive(cereal::make_nvp("maximumPoint", box.maximumPoint));
	archive(cereal::make_nvp("minimumPoint", box.minimumPoint));
}

} // namespace cereal