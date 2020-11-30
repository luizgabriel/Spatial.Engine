#pragma once

#include <spatial/common/Math.h>
#include <spatial/input/InputState.h>

namespace spatial
{

class Input
{
  private:
	static std::shared_ptr<InputState> sInputState;

  public:
	static math::float2 mouse()
	{
		return sInputState->getCurrentPosition();
	}

	static math::float2 lastMouse()
	{
		return sInputState->getLastPosition();
	}

	static math::float2 offset()
	{
		return mouse() - lastMouse();
	}

	static void warpMouse(math::float2 position)
	{
		sInputState->warpMouseInWindow(position);
	}

	static bool pressed(Key key)
	{
		return sInputState->pressed(key);
	}

	template <typename... Args>
	static bool combined(Args... keys)
	{
		return sInputState->combined(keys...);
	}

	static bool released(Key key)
	{
		return sInputState->released(key);
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

	static void with(std::shared_ptr<InputState>& inputState) {
		sInputState = inputState;
	}
};

} // namespace spatial