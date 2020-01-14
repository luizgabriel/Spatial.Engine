#pragma once

#include <spatial/core/Application.h>
#include <spatial/core/ApplicationEvents.h>

namespace spatial::core
{

template <typename Handler>
class SignalsConnector
{
private:
    Application *m_app;
    Handler *m_instance;

public:
    SignalsConnector(Application *app, Handler *instance)
        : m_app{app}, m_instance{instance}
    {
        connect(m_app, m_instance);
    }

    ~SignalsConnector()
    {
        disconnect(m_app, m_instance);
    }

    SignalsConnector(const SignalsConnector &other) = delete;
    SignalsConnector &operator=(const SignalsConnector &other) = delete;
};

template <typename Event, typename Handler>
class EventConnector
{
private:
    Application *m_app;
    Handler *m_instance;

public:
    EventConnector(Application *app, Handler *instance)
        : m_app{app}, m_instance{instance}
    {
        connect<Event>(m_app, m_instance);
    }

    ~EventConnector()
    {
        disconnect<Event>(m_app, m_instance);
    }

    EventConnector(const EventConnector &other) = delete;
    EventConnector &operator=(const EventConnector &other) = delete;
};

} // namespace spatial::core