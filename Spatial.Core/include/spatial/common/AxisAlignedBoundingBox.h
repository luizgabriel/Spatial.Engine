#pragma once

#include <spatial/common/Math.h>

namespace spatial::math
{

template <typename T>
struct BaseAxisAlignedBoundingBox
{
	using PointType = vec<3, T>;

	PointType minimumPoint{std::numeric_limits<T>::max()};
	PointType maximumPoint{std::numeric_limits<T>::lowest()};

	[[nodiscard]] PointType getCenter() const noexcept
	{
		return (maximumPoint + minimumPoint) / 2.0f;
	}

	[[nodiscard]] PointType getExtent() const noexcept
	{
		return (maximumPoint - minimumPoint) / 2.0f;
	}

	[[nodiscard]] bool isEmpty() const noexcept
	{
		return minimumPoint.x >= maximumPoint.x && minimumPoint.y >= maximumPoint.y && minimumPoint.z >= maximumPoint.z;
	}

	[[nodiscard]] BaseAxisAlignedBoundingBox<T> extend(const PointType& point) const noexcept
	{
		return {
			(point.x > maximumPoint.x) && (point.y > maximumPoint.y) && (point.z > maximumPoint.z) ? point
																								   : maximumPoint,
			(point.x < minimumPoint.x) && (point.y < minimumPoint.y) && (point.z < minimumPoint.z) ? point
																								   : minimumPoint,
		};
	}

	[[nodiscard]] float contains(const PointType& point) const noexcept
	{
		float d = minimumPoint.x - point.x;
		d = std::max(d, minimumPoint.y - point.y);
		d = std::max(d, minimumPoint.z - point.z);
		d = std::max(d, point.x - maximumPoint.x);
		d = std::max(d, point.y - maximumPoint.y);
		d = std::max(d, point.z - maximumPoint.z);
		return d;
	}
};

using AxisAlignedBoundingBox = BaseAxisAlignedBoundingBox<float>;

} // namespace spatial::math