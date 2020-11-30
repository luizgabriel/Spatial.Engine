#include <spatial/input/InputSystem.h>

namespace spatial
{

InputSystem::InputSystem() : mInputState{std::make_shared<InputState>()}, mMouseWarpSignal{}
{
	mInputState->reset();
}

void InputSystem::onEndFrame()
{
	if (mInputState->isMouseWarpRequested())
	{
		mMouseWarpSignal.trigger(mInputState->getCurrentPosition());
	}

	mInputState->reset();
}

void InputSystem::onEvent(const MouseMovedEvent& event)
{
	mInputState->setMousePosition({event.x, event.y});
}

void InputSystem::onEvent(const KeyEvent& event)
{
	const auto key = event.key;
	mInputState->set(key, event.action);
}

void InputSystem::onEvent(const TextEvent& event)
{
}

} // namespace spatial
