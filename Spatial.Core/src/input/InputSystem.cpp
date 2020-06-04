#include <spatial/input/InputSystem.h>
#include <spatial/input/Keyboard.h>
#include <spatial/input/Mouse.h>

namespace spatial
{

InputSystem::InputSystem(Window* window) : m_window{window}
{
}

void InputSystem::onStartFrame(float delta)
{
	if (Mouse::s_mouseState.isMouseWarpRequested())
	{
		auto mousePos = Mouse::position();
		auto [windowWidth, windowHeight] = m_window->getWindowSize();
		m_window->warpMouse({mousePos.x * windowWidth, mousePos.y * windowHeight});
	}

	Keyboard::s_keyboardState.reset();
	Mouse::s_mouseState.reset();
}

void InputSystem::onEvent(const MouseMovedEvent& event)
{
	auto [windowWidth, windowHeight] = m_window->getWindowSize();
	Mouse::s_mouseState.set({event.x / windowWidth, event.y / windowHeight});
}

void InputSystem::onEvent(const KeyEvent& event)
{
	const auto key = event.key;
	Keyboard::s_keyboardState.set(key, event.action);
}

void InputSystem::onEvent(const TextEvent& event)
{
}

} // namespace spatial
