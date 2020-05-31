#pragma once

#include <math/vec2.h>
#include <math/vec3.h>
#include <math/mat4.h>
#include <filament/Camera.h>

namespace spatial
{

template <typename ValueType>
constexpr ValueType pi = ValueType{3.14159265359};

template <typename ValueType>
constexpr ValueType halfPi = ValueType{1.57079632679};

struct SimpleCameraView
{
	filament::math::float2 rotation;
	filament::math::float3 position;

	float& yaw()
	{
		return rotation.x;
	}

	float& pitch()
	{
		return rotation.y;
	}

	void onMouseMoved(const filament::math::float2& mousePos, float sensitivity);

	void onUpdate(filament::Camera* camera, float delta);
};

filament::math::float3 toDirection(const filament::math::float2& rot);

} // namespace spatial::math