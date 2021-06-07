#pragma once

#include <cereal/cereal.hpp>
#include <spatial/common/Math.h>

namespace cereal
{

template <typename Archive, typename T>
void serialize(Archive& ar, spatial::math::vec2<T>& vec)
{
	ar(make_nvp("x", vec.x));
	ar(make_nvp("y", vec.y));
}

template <typename Archive, typename T>
void serialize(Archive& ar, spatial::math::vec3<T>& vec)
{
	serialize(ar, vec.xy);
	ar(make_nvp("z", vec.z));
}

template <typename Archive, typename T>
void serialize(Archive& ar, spatial::math::vec4<T>& vec)
{
	serialize(ar, vec.xyz);
	ar(make_nvp("w", vec.w));
}

template <typename Archive, typename T>
void serialize(Archive& ar, spatial::math::mat3_t<T>& mat)
{
	ar(make_nvp("r0", mat[0]));
	ar(make_nvp("r1", mat[1]));
	ar(make_nvp("r2", mat[2]));
}

template <typename Archive, typename T>
void serialize(Archive& ar, spatial::math::mat4_t<T>& mat)
{
	ar(make_nvp("r0", mat[0]));
	ar(make_nvp("r1", mat[1]));
	ar(make_nvp("r2", mat[2]));
	ar(make_nvp("r3", mat[3]));
}

}