#pragma once

#include <spatial/core/Application.h>
#include <spatial/core/ApplicationEvents.h>

namespace spatial::core
{

template <typename Handler>
class ApplicationConnector
{
private:
    Application* m_app;
    Handler *m_instance;
    ApplicationEvents m_flags;

public:
    ApplicationConnector(Application& app, Handler *instance, ApplicationEvents flags = ApplicationEvents::All)
        : m_app{&app}, m_instance{instance}, m_flags{flags}
    {
        connect(*m_app, m_instance, m_flags);
    }

    ~ApplicationConnector()
    {
        disconnect(*m_app, m_instance, m_flags);
    }
};

}