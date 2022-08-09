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

	math::dmat4 projectionMatrix{
		math::perspective(gDefaultVerticalFieldOfView, gWideScreenAspectRatio.get(), defaultNear, defaultFar)};
	double near{defaultNear};
	double far{defaultFar};
};

struct OrthographicCamera
{
	constexpr static auto typeName = "orthographic_camera";

	constexpr static auto defaultNear = .1;
	constexpr static auto defaultFar = 10000.0;

	double left{-gWideScreenAspectRatio.get()};
	double right{gWideScreenAspectRatio.get()};
	double bottom{-1};
	double top{1};

	double near{defaultNear};
	double far{defaultFar};

	constexpr void setAspectRatio(AspectRatio aspectRatio) noexcept
	{
		left = -aspectRatio.get();
		right = aspectRatio.get();
	}
};

struct PerspectiveCamera
{
	constexpr static auto typeName = "perspective_camera";

	constexpr static auto defaultNear = .1;
	constexpr static auto defaultFar = 10000.0;

	double fieldOfView{gDefaultVerticalFieldOfView};
	double aspectRatio{gWideScreenAspectRatio.get()};
	double near{defaultNear};
	double far{defaultFar};
};

} // namespace spatial::ecs