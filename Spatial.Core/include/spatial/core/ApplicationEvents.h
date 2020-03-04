#pragma once

#include <spatial/core/Application.h>
#include <spatial/common/EventQueue.h>
#include <boost/tti/has_member_function.hpp>

namespace spatial::core
{

BOOST_TTI_HAS_MEMBER_FUNCTION(attach);
BOOST_TTI_HAS_MEMBER_FUNCTION(detach);
BOOST_TTI_HAS_MEMBER_FUNCTION(onStart);
BOOST_TTI_HAS_MEMBER_FUNCTION(onStartFrame);
BOOST_TTI_HAS_MEMBER_FUNCTION(onUpdateFrame);
BOOST_TTI_HAS_MEMBER_FUNCTION(onUpdateGuiFrame);
BOOST_TTI_HAS_MEMBER_FUNCTION(onEndFrame);
BOOST_TTI_HAS_MEMBER_FUNCTION(onFinish);

template <typename T>
constexpr bool has_attach_v = has_member_function_attach<T, void, boost::mpl::vector<common::EventQueue&>>::value;

template <typename T>
constexpr bool has_detach_v = has_member_function_detach<T, void, boost::mpl::vector<common::EventQueue&>>::value;

template <typename T>
constexpr bool has_on_start_v = has_member_function_onStart<T, void>::value;

template <typename T>
constexpr bool has_on_start_frame_v = has_member_function_onStartFrame<T, void, boost::mpl::vector<float>>::value;

template <typename T>
constexpr bool has_on_update_frame_v = has_member_function_onUpdateFrame<T, void, boost::mpl::vector<float>>::value;

template <typename T>
constexpr bool has_on_update_gui_frame_v = has_member_function_onUpdateGuiFrame<T, void, boost::mpl::vector<float>>::value;

template <typename T>
constexpr bool has_on_end_frame_v = has_member_function_onEndFrame<T, void, boost::mpl::vector<float>>::value;

template <typename T>
constexpr bool has_on_finish_v = has_member_function_onFinish<T, void>::value;

template <typename Handler>
void connect(Application& app, Handler *instance) noexcept
{
    if constexpr(has_attach_v<Handler>)
        instance->attach(app.getEBus());

    if constexpr(has_on_start_v<Handler>)
        app.getStartSignal().connect<&Handler::onStart>(instance);

    if constexpr(has_on_start_frame_v<Handler>)
        app.getFrameStartSignal().connect<&Handler::onStartFrame>(instance);

    if constexpr(has_on_update_frame_v<Handler>)
        app.getUpdateSignal().connect<&Handler::onUpdateFrame, Handler>(instance);

    if constexpr(has_on_update_gui_frame_v<Handler>)
        app.getUpdateGuiSignal().connect<&Handler::onUpdateGuiFrame, Handler>(instance);

    if constexpr(has_on_end_frame_v<Handler>)
        app.getFrameEndSignal().connect<&Handler::onEndFrame>(instance);

    if constexpr(has_on_finish_v<Handler>)
        app.getFinishSignal().connect<&Handler::onFinish>(instance);
}

template <typename... Handlers>
void connect(Application& app, Handlers*... instance) noexcept
{
    (connect(app, instance), ...);
}

template <typename Handler>
void disconnect(Application& app, Handler *instance) noexcept
{
    if constexpr(has_detach_v<Handler>)
        instance->detach(app.getEBus());

    if constexpr(has_on_start_v<Handler>)
        app.getStartSignal().disconnect<&Handler::onStart>(instance);

    if constexpr(has_on_start_frame_v<Handler>)
        app.getFrameStartSignal().disconnect<&Handler::onStartFrame>(instance);

    if constexpr(has_on_update_frame_v<Handler>)
        app.getUpdateSignal().disconnect<&Handler::onUpdateFrame, Handler>(instance);

    if constexpr(has_on_update_gui_frame_v<Handler>)
        app.getUpdateGuiSignal().disconnect<&Handler::onUpdateGuiFrame, Handler>(instance);

    if constexpr(has_on_end_frame_v<Handler>)
        app.getFrameEndSignal().disconnect<&Handler::onEndFrame>(instance);

    if constexpr(has_on_finish_v<Handler>)
        app.getFinishSignal().disconnect<&Handler::onFinish>(instance);
}

template <typename... Handlers>
void disconnect(Application& app, Handlers&&... instance) noexcept
{
    (disconnect(app, std::forward<Handlers>(instance)), ...);
}

template <typename Event, auto Function>
void connect(Application& app)
{
    app.getEBus().template connect<Event, Function>();
}

template <typename Event, auto Function>
void disconnect(Application& app)
{
    app.getEBus().template disconnect<Event, Function>();
}

template <typename Event, auto Function = nullptr, typename Handler>
void connect(Application& app, Handler *instance)
{
    app.getEBus().template connect<Event, Function>(instance);
}

template <typename Event, auto Function = nullptr, typename Handler>
void disconnect(Application& app, Handler *instance)
{
    app.getEBus().template disconnect<Event, Function>(instance);
}

} // namespace spatial::core