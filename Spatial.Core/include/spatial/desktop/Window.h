#pragma once

#include <string_view>
#include <type_traits>
#include <spatial/common/EventQueue.h>
#include <SDL.h>

namespace spatial::desktop
{

class WindowContext;

class Window
{
private:
	SDL_Window *m_windowHandle;

	Window(int width, int height, std::string_view title);

	friend class WindowContext;

public:
	~Window();

	void onStartRender();
	void onEndRender();

	void* getNativeHandle();

	std::pair<uint32_t, uint32_t> getFrameBufferSize() const;
	std::pair<int, int> getWindowSize() const;

	Window(Window &&other) noexcept;
	Window(const Window &w) = delete;

	Window &operator=(Window &&other) noexcept;
	Window &operator=(const Window &w) = delete;
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