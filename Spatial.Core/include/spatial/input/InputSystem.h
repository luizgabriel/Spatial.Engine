#pragma once

#include <spatial/common/EventQueue.h>
#include <spatial/common/Key.h>
#include <spatial/desktop/PlatformEvent.h>
#include <spatial/desktop/Window.h>

namespace spatial
{

class InputSystem
{
  private:
	Window& mWindow;

  public:
	explicit InputSystem(Window& window);

	InputSystem(const InputSystem& other) = delete;
	InputSystem& operator=(const InputSystem& w) = delete;

	void onStartFrame(float);
	void onEndFrame();

	void onEvent(const MouseMovedEvent& event);
	void onEvent(const KeyEvent& event);
	void onEvent(const TextEvent& event);
};

} // namespace spatial
