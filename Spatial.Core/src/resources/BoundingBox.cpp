#include <filament/Box.h>
#include <spatial/resources/Math.h>

namespace spatial
{

std::istream& operator>>(std::istream& stream, filament::Box& box)
{
	return stream >> box.center >> box.halfExtent;
}

} // namespace spatial