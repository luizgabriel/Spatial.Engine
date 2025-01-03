#pragma once

#include <spatial/common/Math.h>

namespace spatial
{

template <typename T>
struct BaseAspectRatio
{
	T width{1.0};
	T height{1.0};

	constexpr BaseAspectRatio() = default;

	constexpr explicit BaseAspectRatio(math::vec<2, T> dimensions) : width{dimensions.x}, height{dimensions.y}
	{
	}

	constexpr BaseAspectRatio(T width, T height) : width{width}, height{height}
	{
	}

	[[nodiscard]] constexpr auto toVector() const
	{
		return math::vec<2, T>{width, height};
	}

	[[nodiscard]] constexpr T get() const
	{
		return width / height;
	}
};

using AspectRatio = BaseAspectRatio<double>;

template <typename T>
constexpr auto gUltraWideAspectRatio_v = BaseAspectRatio<T>{21, 9};
constexpr auto gUltraWideAspectRatio = gUltraWideAspectRatio_v<double>;

template <typename T>
constexpr auto gWideScreenAspectRatio_v = BaseAspectRatio<T>{16, 9};
constexpr auto gWideScreenAspectRatio = gWideScreenAspectRatio_v<double>;

template <typename T>
constexpr auto gStandardAspectRatio_v = BaseAspectRatio<T>{4, 3};
constexpr auto gStandardAspectRatio = gStandardAspectRatio_v<double>;

template <typename T>
constexpr auto gDefaultVerticalFieldOfView_v = static_cast<T>(45) * math::deg2rad_v<T>;
constexpr auto gDefaultVerticalFieldOfView = gDefaultVerticalFieldOfView_v<double>;

} // namespace spatial