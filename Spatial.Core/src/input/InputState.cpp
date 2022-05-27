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

float InputState::axis(Key positive, Key negative) const
{
	if (pressed(positive))
		return 1.0f;
	else if (pressed(negative))
		return -1.0f;
	else
		return .0f;
}

template <typename... K>
InputState::BitSet bitsetOf(K... key)
{
	auto bit = InputState::BitSet{};
	(bit.set(static_cast<size_t>(key)), ...);
	return bit;
}

bool InputState::combined(Key alt1, Key alt2, Key alt3, Key alt4, Key key) const
{
	return (mKeyPressed == bitsetOf(alt1, alt2, alt3, alt4)) && (mKeyReleased == bitsetOf(key));
}

bool InputState::combined(Key alt1, Key alt2, Key alt3, Key key) const
{
	return (mKeyPressed == bitsetOf(alt1, alt2, alt3)) && (mKeyReleased == bitsetOf(key));
}

bool InputState::combined(Key alt1, Key alt2, Key key) const
{
	return (mKeyPressed == bitsetOf(alt1, alt2)) && (mKeyReleased == bitsetOf(key));
}

bool InputState::combined(Key alt1, Key key) const
{
	return (mKeyPressed == bitsetOf(alt1)) && (mKeyReleased == bitsetOf(key));
}

bool InputState::pressed(Key key) const
{
	return mKeyPressed.test(static_cast<size_t>(key));
}

bool InputState::released(Key key) const
{
	return mKeyReleased.test(static_cast<size_t>(key));
}

} // namespace spatial::desktop