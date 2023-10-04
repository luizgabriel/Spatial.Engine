#pragma once

#include <spatial/common/Key.h>
#include <spatial/common/Math.h>
#include <utility>

namespace spatial::desktop
{

class Window;

struct WindowResizedEvent
{
	const desktop::Window* window;
	math::uvec2 windowSize{};
	math::uvec2 frameBufferSize{};
};

struct WindowClosedEvent
{
	const desktop::Window* window;
};

struct KeyEvent
{
	const desktop::Window* window;
	Key key{Key::UnknownKey};
	KeyAction action{KeyAction::UnknownAction};
};

struct TextEvent
{
	const desktop::Window* window;
	std::string text{};
};

struct MouseMovedEvent
{
	const desktop::Window* window;
	math::dvec2 position{};
};

struct MouseScrolledEvent
{
	const desktop::Window* window;
	math::dvec2 offset{};
};

} // namespace spatial