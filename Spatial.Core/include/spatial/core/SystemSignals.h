#pragma once

#include <spatial/common/Signal.h>

namespace spatial::core
{

class Application;

class SystemSignals
{
private:
    static common::Signal<> s_startSignal;
    static common::Signal<float> s_updateSignal;
    static common::Signal<> s_finishSignal;

public:
    template <typename Type>
    static void connect(Type* instance)
    {
        s_startSignal.template connect<&Type::onStart, Type>(instance);
        s_updateSignal.template connect<&Type::onUpdate, Type>(instance);
        s_finishSignal.template connect<&Type::onFinish, Type>(instance);
    }

    template <typename Type>
    static void disconnect(Type* instance)
    {
        s_startSignal.template disconnect<&Type::onStart, Type>(instance);
        s_updateSignal.template disconnect<&Type::onUpdate, Type>(instance);
        s_finishSignal.template disconnect<&Type::onFinish, Type>(instance);
    }

    template <auto Function>
    static void connect()
    {
        s_startSignal.template connect<Function>();
        s_updateSignal.template connect<Function>();
        s_finishSignal.template connect<Function>();
    }

    template <auto Function>
    static void disconnect()
    {
        s_startSignal.template disconnect<Function>();
        s_updateSignal.template disconnect<Function>();
        s_finishSignal.template disconnect<Function>();
    }

    friend class Application;
};

}