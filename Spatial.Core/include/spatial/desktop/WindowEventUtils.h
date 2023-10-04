#pragma once

#include <spatial/desktop/PlatformEvent.h>
#include <spatial/desktop/Window.h>

namespace spatial
{

template <typename Listener>
void connect(desktop::Window& context, Listener& listener)
{
	auto& eventQueue = context.getEventQueue();
	eventQueue.tryConnect<desktop::WindowResizedEvent>(listener);
	eventQueue.tryConnect<desktop::WindowClosedEvent>(listener);
	eventQueue.tryConnect<desktop::KeyEvent>(listener);
	eventQueue.tryConnect<desktop::TextEvent>(listener);
	eventQueue.tryConnect<desktop::MouseMovedEvent>(listener);
	eventQueue.tryConnect<desktop::MouseScrolledEvent>(listener);
}

template <typename Listener>
void disconnect(desktop::Window& context, Listener& listener)
{
	auto& eventQueue = context.getEventQueue();
	eventQueue.tryDisconnect<desktop::WindowResizedEvent>(listener);
	eventQueue.tryDisconnect<desktop::WindowClosedEvent>(listener);
	eventQueue.tryDisconnect<desktop::KeyEvent>(listener);
	eventQueue.tryDisconnect<desktop::TextEvent>(listener);
	eventQueue.tryDisconnect<desktop::MouseMovedEvent>(listener);
	eventQueue.tryDisconnect<desktop::MouseScrolledEvent>(listener);
}

template <typename Listener>
auto& operator>>(desktop::Window& context, Listener& listener)
{
	connect(context, listener);
	return context;
}

} // namespace spatial