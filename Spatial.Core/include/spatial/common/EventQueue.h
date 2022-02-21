#pragma once

#include <boost/tti/has_member_function.hpp>
#include <entt/signal/dispatcher.hpp>

namespace spatial
{

namespace detail
{
BOOST_TTI_HAS_MEMBER_FUNCTION(onEvent);

template <typename T, typename E>
constexpr bool has_on_event_v = has_member_function_onEvent<T, void, boost::mpl::vector<const E&>>::value;
} // namespace detail

class EventQueue
{
  private:
	entt::dispatcher mDispatcher;

  public:
	template <typename Event>
	void update()
	{
		mDispatcher.update<Event>();
	}

	void update() const
	{
		mDispatcher.update();
	}

	template <typename Event, auto Function>
	void connect()
	{
		mDispatcher.template sink<Event>().template connect<Function>();
	}

	template <typename Event, auto Function = nullptr, typename Listener>
	void connect(Listener& listener)
	{
		auto sink = mDispatcher.template sink<Event>();
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
		mDispatcher.template sink<Event>().template disconnect<Function>();
	}

	template <typename Event, auto Function = nullptr, typename Listener>
	void disconnect(Listener& listener)
	{
		auto sink = mDispatcher.template sink<Event>();
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

	template <typename Event, typename Listener>
	void tryConnect(Listener& listener)
	{
		if constexpr (detail::has_on_event_v<Listener, Event>)
			this->template connect<Event, nullptr, Listener>(listener);
	}

	template <typename Event, typename Listener>
	void tryDisconnect(Listener& listener)
	{
		if constexpr (detail::has_on_event_v<Listener, Event>)
			this->template disconnect<Event>(listener);
	}

	template <typename Event, typename... Args>
	void enqueue(Args&&... args)
	{
		mDispatcher.enqueue<Event, Args...>(std::forward<Args>(args)...);
	}

	template <typename Event>
	void enqueue(Event&& event)
	{
		mDispatcher.enqueue<Event>(std::forward<Event>(event));
	}
};

} // namespace spatial