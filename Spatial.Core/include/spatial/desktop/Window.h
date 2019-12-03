#pragma once

#include <GLFW/glfw3.h>
#include <string_view>
#include <type_traits>
#include <entt/signal/dispatcher.hpp>
#include <spatial/common/EventQueue.h>

namespace spatial::desktop
{

class WindowContext;

class Window
{
private:
	GLFWwindow *m_windowHandle;

	Window(int width, int height, std::string_view title);

	friend class WindowContext;

public:
	~Window();

	void draw();

	bool isClosed() const;

	void swapBuffers();

	void makeCurrentContext();

	std::pair<int, int> getFrameBufferSize() const;

	Window(Window &&other);
	Window(const Window &w) = delete;

	Window &operator=(Window &&other);
	Window &operator=(const Window &w) = delete;

	void setEventQueue(common::EventQueue *queue);
};

class WindowContext
{
private:
	common::EventQueue m_eventQueue;

public:
	WindowContext();
	~WindowContext();

	void pollEvents();

	Window createWindow(int width, int height, std::string_view title);

	WindowContext(const WindowContext &c) = delete;
	WindowContext(const WindowContext &&c) = delete;

	WindowContext &operator=(WindowContext &&other) = delete;
	WindowContext &operator=(const WindowContext &w) = delete;

	template <typename Event, auto Function>
	void connect()
	{
		m_eventQueue.template connect<Event, Function>();
	}

	template <typename Event, auto Function, typename Type>
	void connect(Type *valueOrInstance)
	{
		m_eventQueue.template connect<Event, Function, Type>(valueOrInstance);
	}

	template <typename Event, auto Function>
	void disconnect()
	{
		m_eventQueue.template disconnect<Event, Function>();
	}

	template <typename Event, auto Function, typename Type>
	void disconnect(Type *valueOrInstance)
	{
		m_eventQueue.template disconnect<Event, Function, Type>(valueOrInstance);
	}
};

} // namespace spatial::desktop