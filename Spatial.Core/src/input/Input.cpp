#include <spatial/input/Input.h>

namespace spatial
{

std::shared_ptr<InputState> Input::sInputState{
	std::make_shared<InputState>()
};

}