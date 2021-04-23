#pragma once

#include <spatial/desktop/InputState.h>
#include <spatial/desktop/PlatformEvent.h>
#include <spatial/desktop/Window.h>

namespace spatial::desktop
{

class InputSystem
{
  public:
	InputSystem(const Window& window);

	InputSystem(const InputSystem& other) = delete;
	InputSystem& operator=(const InputSystem& w) = delete;

	void onStartFrame(float delta);

	void onEndFrame();

	void onEvent(const KeyEvent& event);

	const auto& getState()
	{
		return mInputState;
	}

  private:
	InputState mInputState;
	const Window& mWindow;
};

} // namespace spatial
