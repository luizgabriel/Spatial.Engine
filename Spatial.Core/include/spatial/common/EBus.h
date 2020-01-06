#pragma once

#include <spatial/common/EventQueue.h>

namespace  spatial::core { class Application; }

namespace spatial::common
{

class EBus
{
private:
    static EventQueue s_queue;

    template <typename Event>
    static void update()
    {
        s_queue.template update<Event>();
    }

    static void update()
    {
        s_queue.update();
    }

public:
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

    friend class spatial::core::Application;
};

} // namespace spatial::common
