#pragma once

#include <entt/signal/sigh.hpp>

namespace spatial
{

template <typename... Args>
class Signal
{
	using CallbackFn = void(Args...);

  private:
	entt::sigh<CallbackFn> mSigh;
	entt::sink<CallbackFn> mSink;

  public:
	Signal() : mSigh{}, mSink{mSigh}
	{
	}

	void operator()(Args... args) const
	{
		trigger(std::forward<Args>(args)...);
	}

	void trigger(Args... args) const
	{
		mSigh.publish(std::forward<Args>(args)...);
	}

	template <auto Function>
	void connect()
	{
		mSink.template connect<Function>();
	}

	template <auto MemberFunction, typename Listener>
	void connect(Listener& listener)
	{
		mSink.template connect<MemberFunction>(listener);
	}

	template <auto Function>
	void disconnect()
	{
		mSink.template disconnect<Function>();
	}

	template <auto MemberFunction, typename Listener>
	void disconnect(Listener& listener)
	{
		mSink.template disconnect<MemberFunction>(listener);
	}
};

} // namespace spatial