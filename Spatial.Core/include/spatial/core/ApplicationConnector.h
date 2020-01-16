#pragma once

#include <spatial/core/Application.h>
#include <spatial/core/ApplicationEvents.h>
#include <spatial/common/EventConnector.h>

namespace spatial::core
{

template <typename Handler>
class AppSignalsConnector
{
private:
    Application *m_app;
    Handler *m_handler;

public:
    AppSignalsConnector(Application &app, Handler *handler)
        : m_app{&app}, m_handler{handler}
    {
        connect(*m_app, m_handler);
    }

    ~AppSignalsConnector()
    {
        disconnect(*m_app, m_handler);
    }

    AppSignalsConnector(const AppSignalsConnector &other) = delete;
    AppSignalsConnector &operator=(const AppSignalsConnector &other) = delete;
};

template <typename Event, typename Handler>
class AppEventConnector : public common::EventConnector<Event, Handler>
{
    using Base = common::EventConnector<Event, Handler>;

public:
    AppEventConnector(Application &app, Handler *instance)
        : Base::EventConnector(app.getEBus(), instance)
    {
    }
};

} // namespace spatial::core