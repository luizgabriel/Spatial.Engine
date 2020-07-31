#pragma once

#include <spatial/ui/UserInterfaceUtils.h>

namespace spatial
{

template <typename Listener>
void connect(UserInterfaceSystem& ui, Listener& listener)
{
	ui.getDrawGuiSignal().connect<&Listener::onDrawGui>(listener);
}

template <typename... Listeners>
void connect(UserInterfaceSystem& ui, Listeners&... listener)
{
	(connect(ui, listener), ...);
}

template <typename Listener>
void disconnect(UserInterfaceSystem& ui, Listener& listener)
{
	ui.getDrawGuiSignal().disconnect<&Listener::onDrawGui>(listener);
}

template <typename... Listeners>
void disconnect(UserInterfaceSystem& ui, Listeners&... listener)
{
	(disconnect(ui, listener), ...);
}

template <typename Listener>
UserInterfaceSystem& operator>>(UserInterfaceSystem& ui, Listener& listener)
{
	connect(ui, listener);
	return ui;
}

}