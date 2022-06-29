#pragma once

#include <filament/Box.h>
#include <math/mat2.h>
#include <math/mat3.h>
#include <math/mat4.h>
#include <math/vec2.h>
#include <math/vec3.h>
#include <math/vec4.h>
#include <spatial/common/AxisAlignedBoundingBox.h>
#include <spatial/common/Math.h>

namespace spatial
{

/** ---- Vector 2 ---- **/

template <typename T>
inline filament::math::vec2<T> toFilament(const math::vec<2, T>& vec)
{
	return {vec.x, vec.y};
}

template <typename T>
inline math::vec<2, T> fromFilament(const filament::math::vec2<T>& vec)
{
	return {vec.x, vec.y};
}

/** ---- Vector 3 ---- **/

template <typename T>
inline filament::math::vec3<T> toFilament(const math::vec<3, T>& vec)
{
	return {vec.x, vec.y, vec.z};
}

template <typename T>
inline math::vec<3, T> fromFilament(const filament::math::vec3<T>& vec)
{
	return {vec.x, vec.y, vec.z};
}

/** ---- Vector 4 ---- **/

template <typename T>
inline filament::math::vec4<T> toFilament(const math::vec<4, T>& vec)
{
	return {vec.x, vec.y, vec.z, vec.w};
}

template <typename T>
inline math::vec<4, T> fromFilament(const filament::math::vec4<T>& vec)
{
	return {vec.x, vec.y, vec.z, vec.w};
}

/** ---- Matrix 2x2 ---- **/

template <typename T>
inline filament::math::details::TMat22<T> toFilament(const math::mat<2, 2, T>& mat)
{
	return {toFilament(mat[0]), toFilament(mat[1])};
}

template <typename T>
inline math::mat<2, 2, T> fromFilament(const filament::math::details::TMat22<T>& mat)
{
	return {fromFilament(mat[0]), fromFilament(mat[1])};
}

/** ---- Matrix 3x3 ---- **/

template <typename T>
inline filament::math::details::TMat33<T> toFilament(const math::mat<3, 3, T>& mat)
{
	return {toFilament(mat[0]), toFilament(mat[1]), toFilament(mat[2])};
}

template <typename T>
inline math::mat<3, 3, T> fromFilament(const filament::math::details::TMat33<T>& mat)
{
	return {fromFilament(mat[0]), fromFilament(mat[1]), fromFilament(mat[2])};
}

/** ---- Matrix 4x4 ---- **/

template <typename T>
inline filament::math::details::TMat44<T> toFilament(const math::mat<4, 4, T>& mat)
{
	return {toFilament(mat[0]), toFilament(mat[1]), toFilament(mat[2]), toFilament(mat[3])};
}

template <typename T>
inline math::mat<4, 4, T> fromFilament(const filament::math::details::TMat44<T>& mat)
{
	return {fromFilament(mat[0]), fromFilament(mat[1]), fromFilament(mat[2]), fromFilament(mat[3])};
}

/** ---- Bounding Box ---- **/

template <typename T>
inline filament::Box toFilament(const math::BaseAxisAlignedBoundingBox<T>& aabb)
{
	return {toFilament(aabb.getCenter()), toFilament(aabb.getExtent())};
}

inline math::AxisAlignedBoundingBox fromFilament(const filament::Box& box)
{
	return {fromFilament(box.getMin()), fromFilament(box.getMax())};
}

} // namespace spatial