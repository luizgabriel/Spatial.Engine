#include "spatial/input/InputSystem.h"

namespace spatial::desktop
{

InputSystem::InputSystem() : mInputState{}, mOnChangeStateSignal{}
{
}

void InputSystem::onUpdateFrame(float delta)
{
	mOnChangeStateSignal.trigger(mInputState);
	mInputState.approximateMouseOffset(delta);
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

void InputSystem::onEvent(const MouseMovedEvent& event)
{
	mInputState.setMousePosition({event.x, event.y});
}

} // namespace spatial::desktop
