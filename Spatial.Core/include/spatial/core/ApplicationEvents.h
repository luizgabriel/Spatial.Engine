#pragma once

#include <spatial/core/Application.h>
#include <spatial/common/EnumBitMasks.h>

namespace spatial::core
{

enum class ApplicationEvents: std::uint8_t
{
    None = 0,
    OnStart = 1,
    OnUpdate = 2,
    OnFinish = 4,
    All = 7
};


template<typename Type>
void connect(Application& app, Type* instance, ApplicationEvents flags = ApplicationEvents::All)
{
    if (check(flags, ApplicationEvents::OnStart)) {
        app.onStartSignal.template connect<&Type::onStart, Type>(instance);
    }

    if (check(flags, ApplicationEvents::OnUpdate)) {
        app.onUpdateSignal.template connect<&Type::onUpdate, Type>(instance);
    }

    if (check(flags, ApplicationEvents::OnFinish)) {
        app.onFinishSignal.template connect<&Type::onFinish, Type>(instance);
    }
}

template<typename Type>
void disconnect(Application& app, Type* instance, ApplicationEvents flags = ApplicationEvents::All)
{
    if (check(flags, ApplicationEvents::OnStart)) {
        app.onStartSignal.template disconnect<&Type::onStart, Type>(instance);
    }

    if (check(flags, ApplicationEvents::OnUpdate)) {
        app.onUpdateSignal.template disconnect<&Type::onUpdate, Type>(instance);
    }

    if (check(flags, ApplicationEvents::OnFinish)) {
        app.onFinishSignal.template disconnect<&Type::onFinish, Type>(instance);
    }
}

}

ENABLE_BITMASK_OPERATORS(spatial::core::ApplicationEvents)