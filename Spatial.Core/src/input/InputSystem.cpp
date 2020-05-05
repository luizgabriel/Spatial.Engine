#include <spatial/input/InputSystem.h>
#include <spatial/input/Keyboard.h>
#include <spatial/input/Mouse.h>

namespace spatial
{

InputSystem::InputSystem(Window* window)
	: m_window{window}
{
}

void InputSystem::attach(EventQueue& queue)
{
	queue.connect<MouseMovedEvent>(this);
	queue.connect<KeyEvent>(this);
	queue.connect<TextEvent>(this);
}

void InputSystem::detach(EventQueue& queue)
{
	queue.disconnect<MouseMovedEvent>(this);
	queue.disconnect<KeyEvent>(this);
	queue.disconnect<TextEvent>(this);
}

void InputSystem::onStartFrame(float delta)
{
	if (Mouse::s_mouseState.isMouseWarpRequested()) {
		m_window->warpMouse(Mouse::position());
	}

	Keyboard::s_keyboardState.reset();
	Mouse::s_mouseState.reset();
}

void InputSystem::onEvent(const MouseMovedEvent& event)
{
	Mouse::s_mouseState.set({event.x, event.y});
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
