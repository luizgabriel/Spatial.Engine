#include <filament/Box.h>
#include <spatial/common/Math.h>
#include <istream>

namespace spatial
{

template <typename OutputType>
OutputType fromEmbed(const char* data, std::size_t size);

}

namespace std
{

template <typename T>
istream& operator>>(istream& stream, spatial::math::vec2<T>& vector)
{
	stream.read(reinterpret_cast<char*>(&vector[0]), sizeof(T));
	stream.read(reinterpret_cast<char*>(&vector[1]), sizeof(T));

	return stream;
}

template <typename T>
istream& operator>>(istream& stream, spatial::math::vec3<T>& vector)
{
	stream.read(reinterpret_cast<char*>(&vector[0]), sizeof(T));
	stream.read(reinterpret_cast<char*>(&vector[1]), sizeof(T));
	stream.read(reinterpret_cast<char*>(&vector[2]), sizeof(T));

	return stream;
}

istream& operator>>(istream& stream, filament::Box& box);

}