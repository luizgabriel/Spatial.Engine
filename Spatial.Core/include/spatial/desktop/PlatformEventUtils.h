#pragma once

#include <spatial/desktop/PlatformEvent.h>
#include <spatial/desktop/Window.h>

namespace spatial
{

template <typename Listener>
void connect(DesktopPlatformContext& context, Listener& listener)
{
	auto& eventQueue = context.getEventQueue();
	eventQueue.tryConnect<WindowResizedEvent>(listener);
	eventQueue.tryConnect<WindowClosedEvent>(listener);
	eventQueue.tryConnect<KeyEvent>(listener);
	eventQueue.tryConnect<TextEvent>(listener);
	eventQueue.tryConnect<MouseMovedEvent>(listener);
	eventQueue.tryConnect<MouseScrolledEvent>(listener);
}

template <typename... Listeners>
void connect(DesktopPlatformContext& context, Listeners&... listener)
{
	(connect(context, listener), ...);
}

template <typename Listener>
void disconnect(DesktopPlatformContext& context, Listener& listener)
{
	auto& eventQueue = context.getEventQueue();
	eventQueue.tryDisconnect<WindowResizedEvent>(listener);
	eventQueue.tryDisconnect<WindowClosedEvent>(listener);
	eventQueue.tryDisconnect<KeyEvent>(listener);
	eventQueue.tryDisconnect<TextEvent>(listener);
	eventQueue.tryDisconnect<MouseMovedEvent>(listener);
	eventQueue.tryDisconnect<MouseScrolledEvent>(listener);
}

template <typename... Listeners>
void disconnect(DesktopPlatformContext& context, Listeners&... listener)
{
	(disconnect(context, listener), ...);
}

template <typename Listener>
DesktopPlatformContext& operator>>(DesktopPlatformContext& context, Listener& listener)
{
	connect(context, listener);
	return context;
}

} // namespace spatial