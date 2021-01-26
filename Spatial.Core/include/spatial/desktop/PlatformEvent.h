#pragma once

#include <spatial/common/Key.h>
#include <utility>

namespace spatial
{

struct WindowResizedEvent
{
	std::pair<int, int> windowSize;
	std::pair<int, int> frameBufferSize;

	WindowResizedEvent(std::pair<int, int> windowSize, std::pair<int, int> frameBufferSize)
		: windowSize{std::move(windowSize)}, frameBufferSize{std::move(frameBufferSize)}
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
	int repeat;

	explicit KeyEvent(const Key key, const KeyAction action = KeyAction::None, int repeat = 0)
		: key{key}, action{action}, repeat{repeat}
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