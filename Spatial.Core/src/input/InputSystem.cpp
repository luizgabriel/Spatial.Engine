#include <spatial/input/InputSystem.h>
#include <spatial/input/Input.h>


namespace spatial
{

InputSystem::InputSystem()
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
	Input::s_inputState.reset();
}

void InputSystem::onEvent(const MouseMovedEvent& event)
{
	Input::s_inputState.setMousePosition({event.x, event.y});
}

void InputSystem::onEvent(const KeyEvent& event)
{
	auto key = event.key;
	Input::s_inputState.set(key, event.action);
}

void InputSystem::onEvent(const TextEvent& event)
{
}

} // namespace spatial
