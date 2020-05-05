#pragma once

#include <spatial/input/MouseState.h>

namespace spatial
{

class InputSystem;

class Mouse
{
private:
	static MouseState s_mouseState;

public:
	static filament::math::float2 position() {
		return s_mouseState.getCurrentPosition();
	}

	static filament::math::float2 lastPosition() {
		return s_mouseState.getLastPosition();
	}

	static filament::math::float2 offset() {
		return position() - lastPosition();
	}

	friend class InputSystem;
};

} // namespace spatial