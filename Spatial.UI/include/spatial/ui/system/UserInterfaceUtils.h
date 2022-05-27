#pragma once

namespace spatial
{

template <typename Listener>
void connect(ui::UserInterfaceSystem& ui, Listener& listener)
{
	ui.getDrawGuiSignal().connect<&Listener::onDrawGui>(listener);
}

template <typename Listener>
void disconnect(ui::UserInterfaceSystem& ui, Listener& listener)
{
	ui.getDrawGuiSignal().disconnect<&Listener::onDrawGui>(listener);
}

template <typename Listener>
auto& operator>>(ui::UserInterfaceSystem& ui, Listener& listener)
{
	connect(ui, listener);
	return ui;
}

} // namespace spatial