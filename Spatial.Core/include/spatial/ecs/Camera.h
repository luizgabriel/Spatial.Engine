#pragma once

#include <spatial/common/Math.h>

namespace spatial::ecs
{

namespace tags
{

struct IsCamera
{
};

} // namespace tags

struct CustomCamera
{
	math::mat4 projectionMatrix{};
	double near{0.1};
	double far{10000.0};

	constexpr explicit CustomCamera(math::mat4 projection = math::mat4::perspective(60.0, 19.0 / 6.0, 0.1, 10000.0),
									double near = 0.1, double far = 10000.0)
		: projectionMatrix{projection}, near{near}, far{far}
	{
	}
};

struct OrthographicCamera
{
	double left{-1.0};
	double right{1.0};
	double bottom{-1.0};
	double top{1.0};

	double near{0.1};
	double far{1.0};

	constexpr OrthographicCamera(double left, double right, double bottom, double top, double near, double far)
		: left{left}, right{right}, bottom{bottom}, top{top}, near{near}, far{far}
	{
	}

	constexpr OrthographicCamera(double aspectRatio, double near, double far)
		: OrthographicCamera(-aspectRatio, aspectRatio, -1, 1, near, far)
	{
	}

	constexpr OrthographicCamera() = default;

	constexpr void setAspectRatio(double aspectRatio) noexcept
	{
		left = -aspectRatio;
		right = aspectRatio;
	}
};

struct PerspectiveCamera
{
	double fieldOfView{60.0};
	double aspectRatio{19.0 / 6.0};

	double near{0.1};
	double far{10000.0};

	constexpr PerspectiveCamera() = default;

	constexpr PerspectiveCamera(double fieldOfView, double aspectRatio, double near, double far)
		: fieldOfView{fieldOfView}, aspectRatio{aspectRatio}, near{near}, far{far}
	{
	}
};

} // namespace spatial::ecs