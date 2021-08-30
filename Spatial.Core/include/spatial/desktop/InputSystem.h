#pragma once

#include <spatial/desktop/InputState.h>
#include <spatial/desktop/PlatformEvent.h>
#include <spatial/desktop/Window.h>

namespace spatial::desktop
{

class InputSystem
{
  public:
	explicit InputSystem(const Window& window);

	InputSystem(const InputSystem& other) = delete;
	InputSystem& operator=(const InputSystem& w) = delete;

	void onStartFrame(float delta);

	void onUpdateFrame(float delta);

	void onEndFrame();

	void onEvent(const KeyEvent& event);

	auto& getOnChangeStateSignal()
	{
		return mOnChangeStateSignal;
	}

  private:
	const Window& mWindow;
	InputState mInputState;

	Signal<const InputState&> mOnChangeStateSignal;
};

} // namespace spatial
