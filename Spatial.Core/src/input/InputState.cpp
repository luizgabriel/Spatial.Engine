#include <spatial/input/Input.h>

using namespace filament::math;

namespace spatial
{

InputState::InputState() : mKeyPressed{}, mKeyReleased{}
{
}

void InputState::set(const Key key, const KeyAction action)
{
	switch (action)
	{
	case KeyAction::Pressed: setPressed(key); break;

	case KeyAction::Released: setReleased(key); break;

	case KeyAction::None: break;
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
	m_mouseWarpRequested = false;
}

void InputState::setMousePosition(float2 position)
{
	m_lastMousePosition = m_currentMousePosition;
	m_currentMousePosition = position;
}

void InputState::warpMouseInWindow(float2 position)
{
	m_mouseWarpRequested = true;
	setMousePosition(position);
}


} // namespace spatial