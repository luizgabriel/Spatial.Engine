#pragma once

#include <spatial/desktop/Window.h>
#include <spatial/desktop/PlatformEvent.h>
#include <spatial/common/EventQueue.h>
#include <spatial/common/Key.h>

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

	void onStartFrame(float delta);

	void onEvent(const MouseMovedEvent& event);
	void onEvent(const KeyEvent& event);
	void onEvent(const TextEvent& event);
};

} // namespace spatial
