#include <spatial/common/Math.h>

namespace spatial::math
{

template <typename Archive, typename T>
void serialize(Archive& ar, vec2<T>& vec)
{
	ar(vec.x, vec.y);
}

template <typename Archive, typename T>
void serialize(Archive& ar, vec3<T>& vec)
{
	ar(vec.x, vec.y, vec.z);
}

template <typename Archive, typename T>
void serialize(Archive& ar, vec4<T>& vec)
{
	ar(vec.x, vec.y, vec.z, vec.w);
}

template <typename Archive, typename T>
void serialize(Archive& ar, mat3_t<T>& mat)
{
	ar(mat[0], mat[1], mat[2]);
}

template <typename Archive, typename T>
void serialize(Archive& ar, mat4_t<T>& mat)
{
	ar(mat[0], mat[1], mat[2], mat[3]);
}

}