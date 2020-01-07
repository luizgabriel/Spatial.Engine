#pragma once

#include <spatial/common/EventQueue.h>

namespace spatial::core
{

class EBus
{
private:
    static common::EventQueue s_queue;

public:
    template <typename Event>
    static void update()
    {
        s_queue.template update<Event>();
    }

    static void update()
    {
        s_queue.update();
    }

    template <typename Event, auto Function>
    static void connect()
    {
        s_queue.template connect<Event, Function>();
    }

    template <typename Event, auto Function = nullptr, typename Type>
    static void connect(Type *valueOrInstance)
    {
        s_queue.template connect<Event, Function, Type>(valueOrInstance);
    }

    template <typename Event, auto Function>
    static void disconnect()
    {
        s_queue.template disconnect<Event, Function>();
    }

    template <typename Event, auto Function = nullptr, typename Type>
    static void disconnect(Type *valueOrInstance)
    {
        s_queue.template disconnect<Event, Function, Type>(valueOrInstance);
    }

    template <typename Event, typename... Args>
    static void enqueue(Args &&... args)
    {
        s_queue.template enqueue<Event, Args...>(std::template forward<Args>(args)...);
    }

    template <typename Event>
    static void enqueue(Event &&event)
    {
        s_queue.template enqueue<Event>(std::template forward<Event>(event));
    }
};

} // namespace spatial::common
