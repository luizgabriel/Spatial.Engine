#include <spatial/input/Input.h>
#include <spatial/input/InputSystem.h>

namespace spatial
{

InputSystem::InputSystem(Window& window) : mWindow{window}
{
}

void InputSystem::onStartFrame(float)
{
	if (Input::sInputState.isMouseWarpRequested())
	{
		auto mousePos = Input::mouse();
		auto [windowWidth, windowHeight] = mWindow.getWindowSize();
		mWindow.warpMouse({mousePos.x * windowWidth, mousePos.y * windowHeight});
	}
}

void InputSystem::onEndFrame()
{
	Input::sInputState.reset();
}

void InputSystem::onEvent(const MouseMovedEvent& event)
{
	auto [windowWidth, windowHeight] = mWindow.getWindowSize();
	Input::sInputState.setMousePosition({event.x / windowWidth, event.y / windowHeight});
}

void InputSystem::onEvent(const KeyEvent& event)
{
	const auto key = event.key;
	Input::sInputState.set(key, event.action);
}

void InputSystem::onEvent(const TextEvent& event)
{
}

} // namespace spatial
