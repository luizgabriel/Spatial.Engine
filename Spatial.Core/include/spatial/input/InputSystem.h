#pragma once

#include <spatial/desktop/Window.h>
#include <spatial/desktop/PlatformEvent.h>
#include <spatial/common/EventQueue.h>


namespace spatial
{

class InputSystem
{
private:
	Window* m_window;

public:
	explicit InputSystem(Window* window);
	InputSystem(const InputSystem& other) = delete;

	void attach(EventQueue& queue);
	void detach(EventQueue& queue);

	void onStartFrame(float delta);

	void onEvent(const MouseMovedEvent& event);
	void onEvent(const KeyEvent& event);
	void onEvent(const TextEvent& event);
};

} // namespace spatial
