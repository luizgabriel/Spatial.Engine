#include <spatial/input/InputSystem.h>
#include <spatial/input/Input.h>

namespace spatial
{

InputSystem::InputSystem(Window* window) : m_window{window}
{
}

void InputSystem::onStartFrame(float delta)
{
	if (Input::s_inputState.isMouseWarpRequested())
	{
		auto mousePos = Input::mouse();
		auto [windowWidth, windowHeight] = m_window->getWindowSize();
		m_window->warpMouse({mousePos.x * windowWidth, mousePos.y * windowHeight});
	}

	Input::s_inputState.reset();
}

void InputSystem::onEvent(const MouseMovedEvent& event)
{
	auto [windowWidth, windowHeight] = m_window->getWindowSize();
	Input::s_inputState.setMousePosition({event.x / windowWidth, event.y / windowHeight});
}

void InputSystem::onEvent(const KeyEvent& event)
{
	const auto key = event.key;
	Input::s_inputState.set(key, event.action);
}

void InputSystem::onEvent(const TextEvent& event)
{
}

} // namespace spatial
