#pragma once

#include <spatial/common/EventQueue.h>

namespace spatial::common
{

class EBus
{
private:
    static common::EventQueue &get() noexcept;

public:
    template <typename Event>
    static void update()
    {
        get().template update<Event>();
    }

    static void update()
    {
        get().update();
    }

    template <typename Event, auto Function>
    static void connect()
    {
        get().template connect<Event, Function>();
    }

    template <typename Event, auto Function = nullptr, typename Type>
    static void connect(Type *valueOrInstance)
    {
        get().template connect<Event, Function, Type>(valueOrInstance);
    }

    template <typename Event, auto Function>
    static void disconnect()
    {
        get().template disconnect<Event, Function>();
    }

    template <typename Event, auto Function = nullptr, typename Type>
    static void disconnect(Type *valueOrInstance)
    {
        get().template disconnect<Event, Function, Type>(valueOrInstance);
    }

    template <typename Event, typename... Args>
    static void enqueue(Args &&... args)
    {
        get().template enqueue<Event, Args...>(std::template forward<Args>(args)...);
    }

    template <typename Event>
    static void enqueue(Event &&event)
    {
        get().template enqueue<Event>(std::template forward<Event>(event));
    }
};

} // namespace spatial::common
