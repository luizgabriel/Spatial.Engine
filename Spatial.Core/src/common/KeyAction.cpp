#include <spatial/common/KeyAction.h>
#include <iostream>

namespace spatial::common
{

std::string nameOf(const KeyAction action)
{
	switch (action)
	{
		default:
		case KeyAction::None: return "TYPED";
		case KeyAction::Pressed: return "PRESSED";
		case KeyAction::Released: return "RELEASED";
	}
}

} // namespace spatial::common