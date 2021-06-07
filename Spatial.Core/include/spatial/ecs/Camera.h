#pragma once

#include <spatial/common/Math.h>

namespace spatial::ecs
{

struct CustomCamera
{
	math::mat4 projectionMatrix;
	double near;
	double far;

	constexpr CustomCamera(math::mat4 projection, double near, double far)
		: projectionMatrix{std::move(projection)}, near{near}, far{far}
	{
	}
};

struct OrthographicCamera
{
	double left;
	double right;
	double bottom;
	double top;

	double near;
	double far;

	constexpr OrthographicCamera(double left, double right, double bottom, double top, double near, double far)
		: left{left}, right{right}, bottom{bottom}, top{top}, near{near}, far{far}
	{
	}

	constexpr OrthographicCamera(double aspectRatio, double near, double far)
		: OrthographicCamera(-aspectRatio, aspectRatio, -1, 1, near, far)
	{
	}
};

struct PerspectiveCamera
{
	double fieldOfView;
	double aspectRatio;

	double near;
	double far;

	constexpr PerspectiveCamera(double fieldOfView, double aspectRatio, double near, double far)
		: fieldOfView{fieldOfView}, aspectRatio{aspectRatio}, near{near}, far{far}
	{
	}
};

} // namespace spatial::ecs