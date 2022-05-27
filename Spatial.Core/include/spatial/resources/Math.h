#pragma once

#include <istream>
#include <spatial/common/Math.h>

namespace spatial
{

template <typename T, math::precision P>
std::istream& operator>>(std::istream& stream, spatial::math::vec<2, T, P>& vector)
{
	stream.read(reinterpret_cast<char*>(&vector[0]), sizeof(T));
	stream.read(reinterpret_cast<char*>(&vector[1]), sizeof(T));

	return stream;
}

template <typename T, math::precision P>
std::istream& operator>>(std::istream& stream, spatial::math::vec<3, T, P>& vector)
{
	stream.read(reinterpret_cast<char*>(&vector[0]), sizeof(T));
	stream.read(reinterpret_cast<char*>(&vector[1]), sizeof(T));
	stream.read(reinterpret_cast<char*>(&vector[2]), sizeof(T));

	return stream;
}

} // namespace spatial
