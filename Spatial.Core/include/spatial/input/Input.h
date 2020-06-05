#pragma once

#include <spatial/input/InputState.h>
#include <math/vec2.h>

namespace spatial
{

class InputSystem;

class Input
{
private:
	static InputState s_inputState;

public:
	static filament::math::float2 mouse()
	{
		return s_inputState.getCurrentPosition();
	}

	static filament::math::float2 lastMouse()
	{
		return s_inputState.getLastPosition();
	}

	static filament::math::float2 offset()
	{
		return mouse() - lastMouse();
	}

	static void warpMouse(filament::math::float2 position)
	{
		s_inputState.warpMouseInWindow(position);
	}

	static bool pressed(Key key)
	{
		return s_inputState.pressed(key);
	}

	template <typename... Args>
	static bool combined(Args... keys)
	{
		return s_inputState.combined(keys...);
	}

	static bool released(Key key)
	{
		return s_inputState.released(key);
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