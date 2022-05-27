#pragma once

#include <istream>
#include <spatial/common/AxisAlignedBoundingBox.h>
#include <spatial/resources/Math.h>

namespace spatial
{

template <typename T, math::precision P>
std::istream& operator>>(std::istream& stream, math::BaseAxisAlignedBoundingBox<T, P>& box)
{
	using PointType = typename math::BaseAxisAlignedBoundingBox<T, P>::PointType;

	PointType center;
	PointType halfExtent;
	stream >> center >> halfExtent;
	box.maximumPoint = center + halfExtent;
	box.minimumPoint = center - halfExtent;

	return stream;
}

}
