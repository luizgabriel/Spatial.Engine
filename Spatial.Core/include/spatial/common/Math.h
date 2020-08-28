#pragma once

#include <math/quat.h>
#include <math/vec2.h>
#include <math/vec3.h>
#include <math/vec4.h>
#include <math/mat2.h>
#include <math/mat3.h>
#include <math/mat4.h>

#include <float.h>
#include <math/scalar.h>


namespace spatial::math
{

using namespace filament::math;

template <typename T>
constexpr T pi = static_cast<T>(F_PI);

template <typename T>
constexpr T epsilon = static_cast<T>(FLT_EPSILON);

template <typename T>
constexpr details::TQuaternion<T> directionToQuaternion(const details::TVec3<T>& dir) {
	float dn = length(dir);
	auto out = details::TQuaternion<T>{};

	if (dn < epsilon<T> * epsilon<T>) {
		out.x = out.y = out.z = 0;
		out.w = 1;
	} else {
		details::TVec3<T> rotAxis = { 0, -dir.z, dir.y };
		if (dot(rotAxis, rotAxis) < epsilon<T> * epsilon<T>) {
			rotAxis.x = rotAxis.y = 0; rotAxis.z = 1;
		}
		float rotAngle = acos(std::clamp(dir.x / dn, -1.0f, 1.0f));
		out = details::TQuaternion<T>::fromAxisAngle(rotAxis, rotAngle);
	}

	return out;
}

template <typename T>
constexpr details::TVec3<T> quaternionToDirection(details::TQuaternion<T>& dir) {
	float3 d = dir * details::TVec3<T>(1, 0, 0);
	return d / length(d);
}

template <typename T>
constexpr T toDegree(T radians)
{
	return radians * (static_cast<T>(180.0) / pi<T>);
}

}