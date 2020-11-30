#pragma once

#include <spatial/input/InputState.h>
#include <spatial/common/Signal.h>
#include <spatial/desktop/PlatformEvent.h>
#include <spatial/desktop/Window.h>

namespace spatial
{

class InputSystem
{
  public:
	InputSystem();

	InputSystem(const InputSystem& other) = delete;
	InputSystem& operator=(const InputSystem& w) = delete;

	void onEndFrame();

	void onEvent(const MouseMovedEvent& event);
	void onEvent(const KeyEvent& event);
	void onEvent(const TextEvent& event);

	auto& getMouseWarpSignal()
	{
		return mMouseWarpSignal;
	}

	auto& getState() {
		return mInputState;
	}

  private:
	std::shared_ptr<InputState> mInputState;
	Signal<math::float2> mMouseWarpSignal;
};

} // namespace spatial
