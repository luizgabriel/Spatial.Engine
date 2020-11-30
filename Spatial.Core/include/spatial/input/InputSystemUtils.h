#pragma once

#include <spatial/input/InputSystem.h>

namespace spatial
{

template <typename Listener>
void connect(InputSystem& input, Listener& listener)
{
	input.getMouseWarpSignal().connect<&Listener::warpMouse>(listener);
}

template <typename Listener>
void disconnect(InputSystem& input, Listener& listener)
{
	input.getMouseWarpSignal().disconnect<&Listener::onMouseWarpRequested>(listener);
}

template <typename Listener>
InputSystem& operator>>(InputSystem& input, Listener& listener)
{
	connect(input, listener);
	return input;
}

}