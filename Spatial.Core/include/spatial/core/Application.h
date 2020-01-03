#pragma once

#include <spatial/desktop/Window.h>
#include <spatial/common/Event.h>
#include <spatial/render/RenderingSubsystem.h>
#include <spatial/input/InputSubsystem.h>
#include <spatial/physics/SimulationSubsystem.h>

namespace spatial::core
{

class Application final
{
private:
    bool m_running;

    desktop::WindowContext m_windowContext;

    //region Subsystems
    input::InputSubsystem m_input;
    render::RenderingSubsystem m_rendering;
    physics::SimulationSubsystem m_simulation;
    //endregion

    //region Events
    common::Event<float> m_updateEvent;
    common::Event<> m_startEvent;
    common::Event<> m_finishEvent;
    //endregion

public:
    Application();

    ~Application();

    Application(const Application &other) = delete;
    Application &operator=(const Application &other) = delete;

    int run();

    void stop();

    bool isRunning() const { return m_running; }

    void onEvent(const desktop::WindowClosedEvent &event);

    template<typename Type>
    void connect(Type* instance)
    {
        connectOnStart(instance);
        connectOnUpdate(instance);
        connectOnFinish(instance);
    }

    template<typename Type>
    void disconnect(Type* instance)
    {
        disconnectOnStart(instance);
        disconnectOnUpdate(instance);
        disconnectOnFinish(instance);
    }

    //region On Start Event
    template<typename Type>
    void connectOnStart(Type* instance)
    {
        m_startEvent.template connect<&Type::onStart, Type>(instance);
    }

    template<typename Type>
    void disconnectOnStart(Type* instance)
    {
        m_startEvent.template disconnect<&Type::onStart, Type>(instance);
    }
    //endregion

    //region On Finish Event
    template<typename Type>
    void connectOnFinish(Type* instance)
    {
        m_finishEvent.template connect<&Type::onFinish, Type>(instance);
    }

    template<typename Type>
    void disconnectOnFinish(Type* instance)
    {
        m_finishEvent.template connect<&Type::onFinish, Type>(instance);
    }
    //endregion

    //region On Update Event
    template<typename Type>
    void connectOnUpdate(Type* instance)
    {
        m_updateEvent.template connect<&Type::onUpdate, Type>(instance);
    }

    template<typename Type>
    void disconnectOnUpdate(Type* instance)
    {
        m_updateEvent.template disconnect<&Type::onUpdate, Type>(instance);
    }
    //endregion

    //region Getters
    render::RenderingSubsystem &getRenderSys()
    {
        return m_rendering;
    }

    const render::RenderingSubsystem &getRenderSys() const
    {
        return m_rendering;
    }
    //endregion Getters
};

} // namespace spatial::core
