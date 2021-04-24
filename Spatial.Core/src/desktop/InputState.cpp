#include <spatial/desktop/InputState.h>

namespace spatial::desktop
{

InputState::InputState()
	: mKeyPressed{},
	  mKeyReleased{},
	  mLastMousePosition{.0f, .0f},
	  mCurrentMousePosition{.0f, .0f}
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
	const auto flag = static_cast<size_t>(key);
	mKeyPressed.set(flag);
	mKeyReleased.reset(flag);
}

void InputState::setReleased(Key key)
{
	const auto flag = static_cast<size_t>(key);
	mKeyPressed.reset(flag);
	mKeyReleased.set(flag);
}

void InputState::reset(Key key)
{
	mKeyReleased.reset(static_cast<size_t>(key));
}

void InputState::reset()
{
	mKeyReleased.reset();
}

void InputState::setMousePosition(math::float2 position)
{
	mLastMousePosition = mCurrentMousePosition;
	mCurrentMousePosition = position;
}

float InputState::axis(Key positive, Key negative) const
{
	if (pressed(positive))
		return 1.0f;
	else if (pressed(negative))
		return -1.0f;
	else
		return .0f;
}

bool InputState::combined(Key alt1, Key alt2, Key alt3, Key alt4, Key key) const
{
	return pressed(alt1) && pressed(alt2) && pressed(alt3) && pressed(alt4) && released(key);
}

bool InputState::combined(Key alt1, Key alt2, Key alt3, Key key) const
{
	return pressed(alt1) && pressed(alt2) && pressed(alt3) && released(key);
}

bool InputState::combined(Key alt1, Key alt2, Key key) const
{
	return pressed(alt1) && pressed(alt2) && released(key);
}

bool InputState::combined(Key alt1, Key key) const
{
	return pressed(alt1) && released(key);
}

bool InputState::pressed(Key key) const
{
	return mKeyPressed.test(static_cast<size_t>(key));
}

bool InputState::released(Key key) const
{
	return mKeyReleased.test(static_cast<size_t>(key));
}

} // namespace spatial