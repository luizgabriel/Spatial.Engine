#pragma once

#include <cereal/cereal.hpp>
#include <spatial/common/Math.h>

namespace cereal
{

template <typename Archive, typename T, spatial::math::precision P>
void serialize(Archive& ar, spatial::math::vec<2, T, P>& vec)
{
	ar(make_nvp("x", vec.x));
	ar(make_nvp("y", vec.y));
}

template <typename Archive, typename T, spatial::math::precision P>
void serialize(Archive& ar, spatial::math::vec<3, T, P>& vec)
{
	ar(make_nvp("x", vec.x));
	ar(make_nvp("y", vec.y));
	ar(make_nvp("z", vec.z));
}

template <typename Archive, typename T, spatial::math::precision P>
void serialize(Archive& ar, spatial::math::vec<4, T, P>& vec)
{
	ar(make_nvp("x", vec.x));
	ar(make_nvp("y", vec.y));
	ar(make_nvp("z", vec.z));
	ar(make_nvp("w", vec.w));
}

template <typename Archive, typename T, spatial::math::precision P>
void serialize(Archive& ar, spatial::math::mat<2, 2, T, P>& mat)
{
	ar(make_nvp("r0", mat[0]));
	ar(make_nvp("r1", mat[1]));
}

template <typename Archive, typename T, spatial::math::precision P>
void serialize(Archive& ar, spatial::math::mat<3, 3, T, P>& mat)
{
	ar(make_nvp("r0", mat[0]));
	ar(make_nvp("r1", mat[1]));
	ar(make_nvp("r2", mat[2]));
}

template <typename Archive, typename T, spatial::math::precision P>
void serialize(Archive& ar, spatial::math::mat<4, 4, T, P>& mat)
{
	ar(make_nvp("r0", mat[0]));
	ar(make_nvp("r1", mat[1]));
	ar(make_nvp("r2", mat[2]));
	ar(make_nvp("r3", mat[3]));
}

} // namespace cereal