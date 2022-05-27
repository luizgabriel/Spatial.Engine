#pragma once

#include <spatial/common/Math.h>

namespace spatial::ecs
{

namespace tags
{

struct IsCamera
{
	constexpr static auto typeName = "tag_is_camera";
};

} // namespace tags

struct CustomCamera
{
	constexpr static auto typeName = "custom_camera";

	math::mat4 projectionMatrix{};
	double near{0.1};
	double far{10000.0};

	constexpr explicit CustomCamera(math::mat4 projection = math::perspective(60.0, 19.0 / 6.0, 0.1, 10000.0),
									double near = 0.1, double far = 10000.0)
		: projectionMatrix{projection}, near{near}, far{far}
	{
	}
};

struct OrthographicCamera
{
	constexpr static auto typeName = "orthographic_camera";

	double left{-10.0};
	double right{10.0};
	double bottom{-10.0};
	double top{10.0};

	double near{-10.0};
	double far{1000.0};

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
	constexpr static auto typeName = "perspective_camera";

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