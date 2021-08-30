#include <spatial/desktop/InputSystem.h>

namespace spatial::desktop
{

InputSystem::InputSystem(const Window& window) : mWindow{window}, mInputState{}
{
}

void InputSystem::onStartFrame(float)
{
	mInputState.setMousePosition(mWindow.getMousePosition());
}

void InputSystem::onUpdateFrame(float)
{
	mOnChangeStateSignal.trigger(mInputState);
}

void InputSystem::onEndFrame()
{
	mInputState.reset();
}

void InputSystem::onEvent(const KeyEvent& event)
{
	const auto key = event.key;
	mInputState.set(key, event.action);
}

} // namespace spatial::desktop
