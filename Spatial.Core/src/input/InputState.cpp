#include <spatial/input/Input.h>

namespace spatial
{

using namespace filament::math;

InputState::InputState() : m_mousePosition{.0f, .0f}, m_keyPressed{}, m_keyReleased{}
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
	m_keyPressed.set(flag);
	m_keyReleased.reset(flag);
}

void InputState::setReleased(Key key)
{
	const auto flag = static_cast<size_t>(key);
	m_keyPressed.reset(flag);
	m_keyReleased.set(flag);
}

void InputState::reset(Key key)
{
	m_keyReleased.reset(static_cast<size_t>(key));
}

void InputState::reset()
{
	m_keyReleased.reset();
}

void InputState::setMousePosition(float2 position)
{
	m_mousePosition = position;
}

} // namespace spatial