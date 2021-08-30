#include <filament/Box.h>
#include <spatial/common/Math.h>
#include <istream>

namespace spatial
{

std::istream& operator>>(std::istream& stream, filament::Box& box);

}


