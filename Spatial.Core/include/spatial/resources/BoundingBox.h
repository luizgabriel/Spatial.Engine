#pragma once

#include <istream>
#include <spatial/common/AxisAlignedBoundingBox.h>
#include <spatial/resources/Math.h>

namespace spatial
{

template <typename T>
std::istream& operator>>(std::istream& stream, math::BaseAxisAlignedBoundingBox<T>& box)
{
	using PointType = typename math::BaseAxisAlignedBoundingBox<T>::PointType;

	PointType center;
	PointType halfExtent;
	stream >> center >> halfExtent;
	box.maximumPoint = center + halfExtent;
	box.minimumPoint = center - halfExtent;

	return stream;
}

} // namespace spatial
