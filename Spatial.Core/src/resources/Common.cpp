#pragma once

#include <spatial/resources/Common.h>

namespace std
{

istream& operator>>(istream& stream, filament::Box& box)
{
	return stream >> box.center >> box.halfExtent;
}

} // namespace std