#pragma once

#include <spatial/core/Application.h>
#include <spatial/common/EventQueue.h>
#include <boost/tti/has_member_function.hpp>

namespace spatial
{
BOOST_TTI_HAS_MEMBER_FUNCTION(attach);

BOOST_TTI_HAS_MEMBER_FUNCTION(detach);

BOOST_TTI_HAS_MEMBER_FUNCTION(onStart);

BOOST_TTI_HAS_MEMBER_FUNCTION(onStartFrame);

BOOST_TTI_HAS_MEMBER_FUNCTION(onUpdateFrame);

BOOST_TTI_HAS_MEMBER_FUNCTION(onEndFrame);

BOOST_TTI_HAS_MEMBER_FUNCTION(onEndGuiFrame);

BOOST_TTI_HAS_MEMBER_FUNCTION(onDrawGui);

BOOST_TTI_HAS_MEMBER_FUNCTION(onFinish);

template <typename T>
constexpr bool has_attach_v = has_member_function_attach<T, void, boost::mpl::vector<EventQueue&>>::value;

template <typename T>
constexpr bool has_detach_v = has_member_function_detach<T, void, boost::mpl::vector<EventQueue&>>::value;

template <typename T>
constexpr bool has_on_start_v = has_member_function_onStart<T, void>::value;

template <typename T>
constexpr bool has_on_start_frame_v = has_member_function_onStartFrame<T, void, boost::mpl::vector<float>>::value;

template <typename T>
constexpr bool has_on_update_frame_v = has_member_function_onUpdateFrame<T, void, boost::mpl::vector<float>>::value;

template <typename T>
constexpr bool has_on_end_frame_v = has_member_function_onEndFrame<T, void>::value;

template <typename T>
constexpr bool has_on_end_gui_frame_v = has_member_function_onEndGuiFrame<T, void>::value;

template <typename T>
constexpr bool has_on_draw_gui_v = has_member_function_onDrawGui<T, void>::value;

template <typename T>
constexpr bool has_on_finish_v = has_member_function_onFinish<T, void>::value;

template <typename Handler>
void connect(Application& app, Handler* instance)
{
	if constexpr (has_attach_v<Handler>)
	{
		instance->attach(app.getEBus());
	}

	if constexpr (has_on_start_v<Handler>)
	{
		app.getStartSignal().connect<&Handler::onStart>(instance);
	}

	if constexpr (has_on_start_frame_v<Handler>)
	{
		app.getStartFrameSignal().connect<&Handler::onStartFrame, Handler>(instance);
	}

	if constexpr (has_on_update_frame_v<Handler>)
	{
		app.getUpdateFrameSignal().connect<&Handler::onUpdateFrame, Handler>(instance);
	}

	if constexpr (has_on_end_frame_v<Handler>)
	{
		app.getEndFrameSignal().connect<&Handler::onEndFrame, Handler>(instance);
	}

	if constexpr (has_on_end_gui_frame_v<Handler>)
	{
		app.getEndGuiFrameSignal().connect<&Handler::onEndGuiFrame, Handler>(instance);
	}

	if constexpr (has_on_draw_gui_v<Handler>)
	{
		app.getDrawGuiSignal().connect<&Handler::onDrawGui>(instance);
	}

	if constexpr (has_on_finish_v<Handler>)
	{
		app.getFinishSignal().connect<&Handler::onFinish>(instance);
	}
}

template <typename... Handlers>
void connect(Application& app, Handlers*... instance)
{
	(connect(app, instance), ...);
}

template <typename Handler>
void disconnect(Application& app, Handler* instance)
{
	if constexpr (has_detach_v<Handler>)
	{
		instance->detach(app.getEBus());
	}

	if constexpr (has_on_start_v<Handler>)
	{
		app.getStartSignal().disconnect<&Handler::onStart>(instance);
	}

	if constexpr (has_on_start_frame_v<Handler>)
	{
		app.getStartFrameSignal().disconnect<&Handler::onStartFrame, Handler>(instance);
	}

	if constexpr (has_on_update_frame_v<Handler>)
	{
		app.getUpdateFrameSignal().disconnect<&Handler::onUpdateFrame, Handler>(instance);
	}

	if constexpr (has_on_end_frame_v<Handler>)
	{
		app.getEndFrameSignal().disconnect<&Handler::onEndFrame, Handler>(instance);
	}

	if constexpr (has_on_end_gui_frame_v<Handler>)
	{
		app.getEndGuiFrameSignal().disconnect<&Handler::onEndGuiFrame, Handler>(instance);
	}

	if constexpr (has_on_draw_gui_v<Handler>)
	{
		app.getDrawGuiSignal().disconnect<&Handler::onDrawGui>(instance);
	}

	if constexpr (has_on_finish_v<Handler>)
	{
		app.getFinishSignal().disconnect<&Handler::onFinish>(instance);
	}
}

template <typename... Handlers>
void disconnect(Application& app, Handlers*... instance)
{
	(disconnect(app, instance), ...);
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
void connect(Application& app, Handler* instance)
{
	app.getEBus().template connect<Event, Function>(instance);
}

template <typename Event, auto Function = nullptr, typename Handler>
void disconnect(Application& app, Handler* instance)
{
	app.getEBus().template disconnect<Event, Function>(instance);
}
} // namespace spatial
