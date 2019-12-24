#pragma once

#include <entt/signal/sigh.hpp>

namespace spatial::common
{

template <typename... Args>
class Event
{
    using CallbackFn = void(Args...);

private:
    entt::sigh<CallbackFn> m_sigh;
    entt::sink<CallbackFn> m_sink;

public:
    Event()
        : m_sigh{}, m_sink{m_sigh}
    {
    }

    void operator()(Args... args) noexcept
    {
        trigger(args...);
    }

    void trigger(Args... args)
    {
        m_sigh.publish(args...);
    }

    template <auto Function>
    void connect()
    {
        m_sink.template connect<Function>();
    }

    template <auto MemberFunction, typename Type>
    void connect(Type *valueOrInstance)
    {
        m_sink.template connect<MemberFunction>(valueOrInstance);
    }

    template <auto Function>
    void disconnect()
    {
        m_sink.template disconnect<Function>();
    }

    template <auto MemberFunction, typename Type>
    void disconnect(Type *valueOrInstance)
    {
        m_sink.template disconnect<MemberFunction>(valueOrInstance);
    }
};

} // namespace spatial::common