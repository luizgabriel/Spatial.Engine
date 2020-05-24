#pragma once

#include <spatial/input/KeyboardState.h>

namespace spatial
{

class InputSystem;

class Keyboard
{
private:
	static KeyboardState s_keyboardState;

public:
	static bool pressed(Key key)
	{
		return s_keyboardState.pressed(key);
	}

	template <typename... Args>
	static bool combined(Args... keys)
	{
		return s_keyboardState.combined(keys...);
	}

	static bool released(Key key)
	{
		return s_keyboardState.released(key);
	}

	friend class InputSystem;
};

} // namespace spatial