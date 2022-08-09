#pragma once

#include <spatial/common/Key.h>
#include <spatial/common/Math.h>
#include <utility>

namespace spatial
{

struct WindowResizedEvent
{
	math::uvec2 windowSize{};
	math::uvec2 frameBufferSize{};
};

struct WindowClosedEvent
{
};

struct KeyEvent
{
	Key key{Key::UnknownKey};
	KeyAction action{KeyAction::UnknownAction};
};

struct TextEvent
{
	std::string text{};
};

struct MouseMovedEvent
{
	math::dvec2 position{};
};

struct MouseScrolledEvent
{
	math::dvec2 offset{};
};

} // namespace spatial