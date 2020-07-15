#pragma once

#include <entt/signal/dispatcher.hpp>

namespace spatial
{

class EventQueue
{
private:
	entt::dispatcher m_dispatcher;

public:
	template <typename Event>
	void update()
	{
		m_dispatcher.update<Event>();
	}

	void update() const
	{
		m_dispatcher.update();
	}

	template <typename Event, auto Function>
	void connect()
	{
		m_dispatcher.template sink<Event>().template connect<Function>();
	}

	template <typename Event, auto Function = nullptr, typename Listener>
	void connect(Listener& listener)
	{
		auto sink = m_dispatcher.template sink<Event>();
		if constexpr (std::is_null_pointer_v<decltype(Function)>)
		{
			constexpr auto overloaded = static_cast<void (Listener::*)(const Event&)>(&Listener::onEvent);
			sink.template connect<overloaded>(listener);
		}
		else
		{
			sink.template connect<Function>(listener);
		}
	}

	template <typename Event, auto Function>
	void disconnect()
	{
		m_dispatcher.template sink<Event>().template disconnect<Function>();
	}

	template <typename Event, auto Function = nullptr, typename Listener>
	void disconnect(Listener& listener)
	{
		auto sink = m_dispatcher.template sink<Event>();
		if constexpr (std::is_null_pointer_v<decltype(Function)>)
		{
			constexpr auto overloaded = static_cast<void (Listener::*)(const Event&)>(&Listener::onEvent);
			sink.template disconnect<overloaded>(listener);
		}
		else
		{
			sink.template disconnect<Function>(listener);
		}
	}

	template <typename Event, typename... Args>
	void enqueue(Args&&... args)
	{
		m_dispatcher.enqueue<Event, Args...>(std::forward<Args>(args)...);
	}

	template <typename Event>
	void enqueue(Event&& event)
	{
		m_dispatcher.enqueue<Event>(std::forward<Event>(event));
	}
};

} // namespace spatial