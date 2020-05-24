#pragma once

#include <spatial/input/MouseState.h>

namespace spatial
{

namespace math = filament::math;

class InputSystem;

class Mouse
{
private:
	static MouseState s_mouseState;

public:
	static math::float2 position()
	{
		return s_mouseState.getCurrentPosition();
	}

	static math::float2 lastPosition()
	{
		return s_mouseState.getLastPosition();
	}

	static math::float2 offset()
	{
		return position() - lastPosition();
	}

	static void move(math::float2 position)
	{
		s_mouseState.warpMouseInWindow(position);
	}

	friend class InputSystem;
};

} // namespace spatial