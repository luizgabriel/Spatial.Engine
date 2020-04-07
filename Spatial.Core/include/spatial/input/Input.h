#pragma once

#include <spatial/input/InputState.h>

namespace spatial
{

class InputSystem;

class Input
{
private:
	static InputState s_inputState;

public:
	static bool pressed(Key key) { return s_inputState.pressed(key); }

	template <typename... Args>
	static bool combined(Args... keys)
	{
		return s_inputState.combined(keys...);
	}

	static bool released(Key key) { return s_inputState.released(key); }

	static filament::math::float2 mouse() { return s_inputState.getMousePosition(); }

	friend class InputSystem;
};

} // namespace spatial::input