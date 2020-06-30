#pragma once

#include <spatial/input/InputState.h>
#include <math/vec2.h>

namespace spatial
{

class InputSystem;

class Input
{
private:
	static InputState sInputState;

public:
	static filament::math::float2 mouse()
	{
		return sInputState.getCurrentPosition();
	}

	static filament::math::float2 lastMouse()
	{
		return sInputState.getLastPosition();
	}

	static filament::math::float2 offset()
	{
		return mouse() - lastMouse();
	}

	static void warpMouse(filament::math::float2 position)
	{
		sInputState.warpMouseInWindow(position);
	}

	static bool pressed(Key key)
	{
		return sInputState.pressed(key);
	}

	template <typename... Args>
	static bool combined(Args... keys)
	{
		return sInputState.combined(keys...);
	}

	static bool released(Key key)
	{
		return sInputState.released(key);
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