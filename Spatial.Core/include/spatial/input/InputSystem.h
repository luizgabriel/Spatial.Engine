#pragma once

#include <spatial/common/Signal.h>
#include <spatial/desktop/PlatformEvent.h>
#include <spatial/input/InputState.h>

namespace spatial::desktop
{

class InputSystem
{
  public:
	InputSystem();

	InputSystem(const InputSystem& other) = delete;
	InputSystem& operator=(const InputSystem& w) = delete;

	void onUpdateFrame(float delta);

	void onEndFrame();

	void onEvent(const KeyEvent& event);

	void onEvent(const MouseMovedEvent& event);

	auto& getOnChangeStateSignal()
	{
		return mOnChangeStateSignal;
	}

  private:
	InputState mInputState;
	Signal<const InputState&> mOnChangeStateSignal;
};

} // namespace spatial::desktop
