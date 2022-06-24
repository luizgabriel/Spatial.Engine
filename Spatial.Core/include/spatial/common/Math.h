#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <limits>

namespace spatial::math
{

using namespace glm;

template <typename T>
constexpr auto epsilon_v = std::numeric_limits<T>::epsilon();
constexpr auto epsilon = epsilon_v<float>;

template <typename T>
constexpr auto pi_v = glm::pi<T>();
constexpr auto pi = pi_v<float>;

template <typename T>
constexpr auto halfPi_v = glm::half_pi<T>();
constexpr auto halfPi = halfPi_v<float>;

constexpr auto axisX = vec3{1.0, .0, .0};
constexpr auto axisY = vec3{.0, 1.0, .0};
constexpr auto axisZ = vec3{.0, .0, 1.0};

template <typename T>
constexpr T rad2deg_v = static_cast<T>(180.0) / pi_v<T>;
constexpr auto rad2deg = rad2deg_v<float>;

template <typename T>
constexpr T deg2rad_v = pi_v<T> / static_cast<T>(180.0);
constexpr auto deg2rad = deg2rad_v<float>;

template <typename T>
qua<T> directionToQuaternion(const vec<3, T>& dir)
{
	auto directionLength = length(dir);
	auto out = quat{};

	if (directionLength < epsilon_v<T> * epsilon_v<T>) {
		out.x = out.y = out.z = 0;
		out.w = 1;
	}
	else {
		auto rotAxis = decltype(dir){0, -dir.z, dir.y};
		if (dot(rotAxis, rotAxis) < epsilon * epsilon) {
			rotAxis.x = rotAxis.y = 0;
			rotAxis.z = 1;
		}
		auto rotAngle = acos(clamp(dir.x / directionLength, -1.0f, 1.0f));
		out = angleAxis(rotAngle, rotAxis);
	}

	return out;
}

template <typename T>
vec<3, T> quaternionToDirection(const qua<T>& dir)
{
	auto d = dir * axisX;
	return d / length(d);
}

template <typename T>
vec<3, T> forwardVector(const mat<4, 4, T>& mat)
{
	return normalize(-math::vec<3, T>{mat[2]});
}
} // namespace spatial::math