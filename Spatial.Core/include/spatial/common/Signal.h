#pragma once

#include <entt/signal/sigh.hpp>

namespace spatial
{

template <typename... Args>
class Signal
{
	using CallbackFn = void(Args...);

private:
	entt::sigh<CallbackFn> m_sigh;
	entt::sink<CallbackFn> m_sink;

public:
	Signal() : m_sigh{}, m_sink{m_sigh}
	{
	}

	void operator()(Args... args) noexcept
	{
		trigger(std::template forward<Args>(args)...);
	}

	void trigger(Args... args)
	{
		m_sigh.publish(std::template forward<Args>(args)...);
	}

	template <auto Function>
	void connect()
	{
		m_sink.template connect<Function>();
	}

	template <auto MemberFunction, typename Type>
	void connect(Type* valueOrInstance)
	{
		m_sink.template connect<MemberFunction>(valueOrInstance);
	}

	template <auto Function>
	void disconnect()
	{
		m_sink.template disconnect<Function>();
	}

	template <auto MemberFunction, typename Type>
	void disconnect(Type* valueOrInstance)
	{
		m_sink.template disconnect<MemberFunction>(valueOrInstance);
	}
};

} // namespace spatial