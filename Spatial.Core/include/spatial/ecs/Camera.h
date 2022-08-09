#pragma once

#include "spatial/common/AspectRatio.h"
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
	constexpr static auto defaultNear = .1;
	constexpr static auto defaultFar = 10000.0;

	math::dmat4 projectionMatrix{};
	double near{defaultNear};
	double far{defaultFar};

	constexpr CustomCamera()
		: projectionMatrix{
			math::perspective(defaultVerticalFieldOfView, wideScreenAspectRatio.get(), defaultNear, defaultFar)}
	{
	}
};

struct OrthographicCamera
{
	constexpr static auto typeName = "orthographic_camera";

	constexpr static auto defaultNear = .1;
	constexpr static auto defaultFar = 10000.0;

	double left{-wideScreenAspectRatio.get()};
	double right{wideScreenAspectRatio.get()};
	double bottom{-1};
	double top{1};

	double near{defaultNear};
	double far{defaultFar};

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

	constexpr static auto defaultNear = .1;
	constexpr static auto defaultFar = 10000.0;

	double fieldOfView{defaultVerticalFieldOfView};
	double aspectRatio{wideScreenAspectRatio.get()};
	double near{defaultNear};
	double far{defaultFar};

	constexpr PerspectiveCamera() = default;
};

} // namespace spatial::ecs