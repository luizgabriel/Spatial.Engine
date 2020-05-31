#pragma once

#include <math/vec2.h>
#include <math/vec3.h>
#include <math/mat4.h>

namespace spatial
{

template <typename ValueType>
constexpr ValueType pi = ValueType{3.14159265359};

template <typename ValueType>
constexpr ValueType halfPi = ValueType{1.57079632679};

struct SimpleCameraView
{
	filament::math::float2 rotation;

	float& yaw()
	{
		return rotation.x;
	}

	float& pitch()
	{
		return rotation.y;
	}

	void update(const filament::math::float2& mousePos, float sensitivity);
};

void incrementCameraRotation(filament::math::float2& rotation, float sensitivity, const filament::math::float2& mousePos);

filament::math::float3 toDirection(const filament::math::float2& rot);

} // namespace spatial::math