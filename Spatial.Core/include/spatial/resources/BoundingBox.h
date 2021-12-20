#include <filament/Box.h>
#include <istream>
#include <spatial/common/Math.h>

namespace spatial
{

std::istream& operator>>(std::istream& stream, filament::Box& box);

}
