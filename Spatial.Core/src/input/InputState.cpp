#include "spatial/input/InputState.h"

namespace spatial::desktop
{

InputState::InputState() : mKeyPressed{}, mKeyReleased{}, mLastMousePosition{.0f}, mCurrentMousePosition{.0f}
{
}

void InputState::set(const Key key, const KeyAction action)
{
	switch (action)
	{
	case KeyAction::Pressed:
		setPressed(key);
		break;

	case KeyAction::Released:
		setReleased(key);
		break;

	case KeyAction::UnknownAction:
		break;
	}
}

void InputState::setPressed(Key key)
{
	auto flag = static_cast<size_t>(key);
	mKeyPressed.set(flag);
	mKeyReleased.reset(flag);
}

void InputState::setReleased(Key key)
{
	auto flag = static_cast<size_t>(key);
	mKeyPressed.reset(flag);
	mKeyReleased.set(flag);
}

void InputState::reset(Key key)
{
	mKeyReleased.reset(static_cast<size_t>(key));
}

void InputState::approximateMouseOffset(float delta)
{
	mLastMousePosition += (mCurrentMousePosition - mLastMousePosition) * delta * 10.0f;
}

void InputState::reset()
{
	mKeyReleased.reset();
}

void InputState::setMousePosition(math::vec2 position)
{
	mLastMousePosition = mCurrentMousePosition;
	mCurrentMousePosition = position;
}

} // namespace spatial::desktop