#pragma once

#include <spatial/common/Key.h>
#include <spatial/common/Math.h>
#include <utility>

namespace spatial
{

struct WindowResizedEvent
{
	math::uvec2 windowSize;
	math::uvec2 frameBufferSize;

	WindowResizedEvent(math::uvec2 windowSize, math::uvec2 frameBufferSize)
		: windowSize{windowSize}, frameBufferSize{frameBufferSize}
	{
	}
};

struct WindowClosedEvent
{
};

struct KeyEvent
{
	Key key;
	KeyAction action;

	explicit KeyEvent(const Key key, const KeyAction action = KeyAction::UnknownAction) : key{key}, action{action}
	{
	}
};

struct TextEvent
{
	std::string text;

	explicit TextEvent(std::string&& text) : text{text}
	{
	}
};

struct MouseMovedEvent
{
	double x, y;
	MouseMovedEvent(double x, double y) : x{x}, y{y}
	{
	}
};

struct MouseScrolledEvent
{
	double xOffset, yOffset;

	MouseScrolledEvent(double x, double y) : xOffset{x}, yOffset{y}
	{
	}
};

} // namespace spatial