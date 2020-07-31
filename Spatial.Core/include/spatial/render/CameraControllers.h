#pragma once

#include <filament/Camera.h>
#include <math/mat4.h>
#include <math/vec2.h>
#include <math/vec3.h>

namespace spatial
{

template <typename ValueType>
constexpr ValueType pi = static_cast<ValueType>(3.14159265359L);

template <typename ValueType>
constexpr ValueType halfPi = static_cast<ValueType>(1.57079632679L);

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

	void onUpdate(filament::Camera& camera, const filament::math::float3& delta);
};

filament::math::float3 toDirection(const filament::math::float2& rot);

} // namespace spatial