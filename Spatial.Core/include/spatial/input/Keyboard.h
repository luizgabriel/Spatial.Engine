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

	static float axis(Key positive, Key negative)
	{
		if (pressed(positive))
			return 1.0f;
		else if (pressed(negative))
			return -1.0f;
		else
			return .0f;
	}

	friend class InputSystem;
};

} // namespace spatial