#pragma once

#include <spatial/core/Application.h>
#include <boost/tti/has_member_function.hpp>

namespace spatial::core
{

BOOST_TTI_HAS_MEMBER_FUNCTION(onStart);
BOOST_TTI_HAS_MEMBER_FUNCTION(onStartFrame);
BOOST_TTI_HAS_MEMBER_FUNCTION(onUpdateFrame);
BOOST_TTI_HAS_MEMBER_FUNCTION(onEndFrame);
BOOST_TTI_HAS_MEMBER_FUNCTION(onFinish);

template <typename T>
constexpr bool has_mem_func_on_start_v = has_member_function_onStart<T, void>::value;

template <typename T>
constexpr bool has_mem_func_on_start_frame_v = has_member_function_onStartFrame<T, void, boost::mpl::vector<float>>::value;

template <typename T>
constexpr bool has_mem_func_on_update_frame_v = has_member_function_onUpdateFrame<T, void, boost::mpl::vector<float>>::value;

template <typename T>
constexpr bool has_mem_func_on_end_frame_v = has_member_function_onEndFrame<T, void, boost::mpl::vector<float>>::value;

template <typename T>
constexpr bool has_mem_func_on_finish_v = has_member_function_onFinish<T, void>::value;

template <typename Handler>
void connect(Application *app, Handler *instance) noexcept
{
    if constexpr(has_mem_func_on_start_v<Handler>)
    {
        app->getStartSignal().connect<&Handler::onStart>(instance);
    }

    if constexpr(has_mem_func_on_start_frame_v<Handler>)
    {
        app->getFrameStartSignal().connect<&Handler::onStartFrame>(instance);
    }

    if constexpr(has_mem_func_on_update_frame_v<Handler>)
    {
        app->getUpdateSignal().connect<&Handler::onUpdateFrame, Handler>(instance);
    }

    if constexpr(has_mem_func_on_end_frame_v<Handler>)
    {
        app->getFrameEndSignal().connect<&Handler::onEndFrame>(instance);
    }

    if constexpr(has_mem_func_on_finish_v<Handler>)
    {
        app->getFinishSignal().connect<&Handler::onFinish>(instance);
    }
}

template <typename Handler>
void disconnect(Application *app, Handler *instance) noexcept
{
    if constexpr(has_mem_func_on_start_v<Handler>)
    {
        app->getStartSignal().disconnect<&Handler::onStart>(instance);
    }

    if constexpr(has_mem_func_on_start_frame_v<Handler>)
    {
        app->getFrameStartSignal().disconnect<&Handler::onStartFrame>(instance);
    }

    if constexpr(has_mem_func_on_update_frame_v<Handler>)
    {
        app->getUpdateSignal().disconnect<&Handler::onUpdateFrame, Handler>(instance);
    }

    if constexpr(has_mem_func_on_end_frame_v<Handler>)
    {
        app->getFrameEndSignal().disconnect<&Handler::onEndFrame>(instance);
    }

    if constexpr(has_mem_func_on_finish_v<Handler>)
    {
        app->getFinishSignal().disconnect<&Handler::onFinish>(instance);
    }
}

template <typename Event, typename Handler>
void connect(Application *app, Handler *instance) noexcept
{
    app->getEBus().template connect<Event>(instance);
}

template <typename Event, typename Handler>
void disconnect(Application *app, Handler *instance) noexcept
{
    app->getEBus().template disconnect<Event>(instance);
}

} // namespace spatial::core