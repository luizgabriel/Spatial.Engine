#pragma once

#include <spatial/desktop/PlatformEvent.h>
#include <spatial/common/EventQueue.h>

namespace spatial
{

class InputSystem
{
public:
	InputSystem();

	InputSystem(const InputSystem& other) = delete;
	InputSystem(InputSystem&& other) = default;

	void attach(EventQueue& queue);
	void detach(EventQueue& queue);

	void onStartFrame(float delta);

	void onEvent(const MouseMovedEvent& event);
	void onEvent(const KeyEvent& event);
	void onEvent(const TextEvent& event);
};

} // namespace spatial
