#include <spatial/input/Keyboard.h>

namespace spatial
{

KeyboardState::KeyboardState() : m_keyPressed{}, m_keyReleased{}
{
}

void KeyboardState::set(const Key key, const KeyAction action)
{
	switch (action)
	{
	case KeyAction::Pressed: setPressed(key); break;

	case KeyAction::Released: setReleased(key); break;

	case KeyAction::None: break;
	}
}

void KeyboardState::setPressed(Key key)
{
	const auto flag = static_cast<size_t>(key);
	m_keyPressed.set(flag);
	m_keyReleased.reset(flag);
}

void KeyboardState::setReleased(Key key)
{
	const auto flag = static_cast<size_t>(key);
	m_keyPressed.reset(flag);
	m_keyReleased.set(flag);
}

void KeyboardState::reset(Key key)
{
	m_keyReleased.reset(static_cast<size_t>(key));
}

void KeyboardState::reset()
{
	m_keyReleased.reset();
}

} // namespace spatial