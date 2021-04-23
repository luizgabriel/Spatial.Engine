#pragma once

#define _USE_MATH_DEFINES
#include <cmath>

#include <limits>
#include <math/mat2.h>
#include <math/mat3.h>
#include <math/mat4.h>
#include <math/quat.h>
#include <math/scalar.h>
#include <math/vec2.h>
#include <math/vec3.h>
#include <math/vec4.h>

namespace spatial::math
{

template <typename T, typename = filament::math::details::enable_if_arithmetic_t<T>>
using vec2 = filament::math::details::TVec2<T>;
using double2 = filament::math::double2;
using float2 = filament::math::float2;
using half2 = filament::math::half2;
using int2 = filament::math::int2;
using uint2 = filament::math::uint2;
using short2 = filament::math::short2;
using ushort2 = filament::math::ushort2;
using byte2 = filament::math::byte2;
using ubyte2 = filament::math::ubyte2;
using bool2 = filament::math::bool2;

template <typename T, typename = filament::math::details::enable_if_arithmetic_t<T>>
using vec3 = filament::math::details::TVec3<T>;
using double3 = filament::math::double3;
using float3 = filament::math::float3;
using half3 = filament::math::half3;
using int3 = filament::math::int3;
using uint3 = filament::math::uint3;
using short3 = filament::math::short3;
using ushort3 = filament::math::ushort3;
using byte3 = filament::math::byte3;
using ubyte3 = filament::math::ubyte3;
using bool3 = filament::math::bool3;

template <typename T, typename = filament::math::details::enable_if_arithmetic_t<T>>
using vec4 = filament::math::details::TVec4<T>;
using double4 = filament::math::double4;
using float4 = filament::math::float4;
using half4 = filament::math::half4;
using int4 = filament::math::int4;
using uint4 = filament::math::uint4;
using short4 = filament::math::short4;
using ushort4 = filament::math::ushort4;
using byte4 = filament::math::byte4;
using ubyte4 = filament::math::ubyte4;
using bool4 = filament::math::bool4;

template <typename T>
using mat2_t = filament::math::details::TMat22<T>;
using mat2 = filament::math::mat2;
using mat2f = filament::math::mat2f;

template <typename T>
using mat3_t = filament::math::details::TMat33<T>;
using mat3 = filament::math::mat3;
using mat3f = filament::math::mat3f;

template <typename T>
using mat4_t = filament::math::details::TMat44<T>;
using mat4 = filament::math::mat4;
using mat4f = filament::math::mat4f;

template <typename T>
using quat_t = filament::math::details::TQuaternion<T>;
using quat = filament::math::quat;
using quatf = filament::math::quatf;

template <typename T>
constexpr auto epsilon_v = std::numeric_limits<T>::epsilon();
constexpr auto epsilon = epsilon_v<float>;

template <typename T>
constexpr auto pi_v = static_cast<T>(M_PI);
constexpr auto pi = pi_v<float>;

template <typename T>
constexpr auto axisX_v = vec3<T>{1.0, .0, .0};

template <typename T>
constexpr auto axisY_v = vec3<T>{.0, 1.0, .0};

template <typename T>
constexpr auto axisZ_v = vec3<T>{.0, .0, 1.0};

constexpr auto axisX = axisX_v<float>;

constexpr auto axisY = axisY_v<float>;

constexpr auto axisZ = axisZ_v<float>;

template <typename T>
constexpr T rad2deg_v = static_cast<T>(180.0) / pi_v<T>;
constexpr auto rad2deg = rad2deg_v<float>;

template <typename T>
constexpr T deg2rad_v = pi_v<T> / static_cast<T>(180.0);
constexpr auto deg2rad = deg2rad_v<float>;

template <typename T>
constexpr quat_t<T> directionToQuaternion(const vec3<T>& dir)
{
	auto dn = length(dir);
	auto out = quat_t<T>{};

	if (dn < epsilon_v<T> * epsilon_v<T>)
	{
		out.x = out.y = out.z = 0;
		out.w = 1;
	}
	else
	{
		vec3<T> rotAxis = {0, -dir.z, dir.y};
		if (dot(rotAxis, rotAxis) < epsilon_v<T> * epsilon_v<T>)
		{
			rotAxis.x = rotAxis.y = 0;
			rotAxis.z = 1;
		}
		auto rotAngle = acos(std::clamp(dir.x / dn, -1.0f, 1.0f));
		out = quat_t<T>::fromAxisAngle(rotAxis, rotAngle);
	}

	return out;
}

template <typename T>
constexpr vec3<T> quaternionToDirection(const quat_t<T>& dir)
{
	auto d = dir * axisX_v<T>;
	return d / length(d);
}

template <typename T>
constexpr vec3<T> forwardVector(const mat4_t<T>& mat)
{
	return normalize(-mat[2].xyz);
}

/**
 * See more: http://planning.cs.uiuc.edu/node103.html
 * @tparam T
 * @param mat
 * @return (roll, yaw, pitch)
 */
template <typename T>
constexpr vec3<T> eulerZYX(const mat4_t<T>& mat)
{
	const auto yaw = std::atan2(mat[2][1], mat[1][1]);
	const auto pitch = std::atan2(-mat[3][1], std::sqrt(mat[3][2] * mat[3][2] + mat[3][3] * mat[3][3]));
	const auto roll = std::atan2(mat[3][2], mat[3][3]);

	return {roll, yaw, pitch};
}

/**
 *
 * @tparam T
 * @param mat
 * @return (pitch, yaw, roll)
 */
template <typename T>
constexpr vec3<T> eulerYXZ(const mat4_t<T>& mat)
{
	const auto ryp = eulerZYX(mat);
	return {ryp.z, ryp.y, ryp.x};
}

} // namespace spatial::math