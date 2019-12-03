#pragma once

#include <entt/signal/dispatcher.hpp>

namespace spatial::common
{

class EventQueue
{
private:
    entt::dispatcher m_dispatcher;

public:
    template <typename Event>
    void update()
    {
        m_dispatcher.template update<Event>();
    }

    void update()
    {
        m_dispatcher.update();
    }

    template <typename Event, auto Function>
    void connect()
    {
        m_dispatcher.template sink<Event>().template connect<Function>();
    }

    template <typename Event, auto Function, typename Type>
    void connect(Type *valueOrInstance)
    {
        m_dispatcher.template sink<Event>().template connect<Function, Type>(valueOrInstance);
    }

    template <typename Event, auto Function>
    void disconnect()
    {
        m_dispatcher.template sink<Event>().template disconnect<Function>();
    }

    template <typename Event, auto Function, typename Type>
    void disconnect(Type *valueOrInstance)
    {
        m_dispatcher.template sink<Event>().template disconnect<Function, Type>(valueOrInstance);
    }

    template <typename Event, typename... Args>
    void enqueue(Args &&... args)
    {
        m_dispatcher.template enqueue<Event, Args...>(std::template forward<Args>(args)...);
    }

    template <typename Event>
    void enqueue(Event &&event)
    {
        m_dispatcher.template enqueue<Event>(std::template forward<Event>(event));
    }
};

} // namespace spatial::common