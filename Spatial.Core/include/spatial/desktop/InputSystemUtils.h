#pragma once

#include <spatial/desktop/InputSystem.h>

namespace spatial
{

template <typename Listener>
void connect(desktop::InputSystem& system, Listener& listener)
{
	system.getOnChangeStateSignal().connect<&Listener::onUpdateInput>(listener);
}

template <typename Listener>
void disconnect(desktop::InputSystem& system, Listener& listener)
{
	system.getOnChangeStateSignal().disconnect<&Listener::onUpdateInput>(listener);
}

template <typename Listener>
desktop::InputSystem& operator >>(desktop::InputSystem& system, Listener& listener)
{
	connect(system, listener);
	return system;
}

} // namespace spatial
