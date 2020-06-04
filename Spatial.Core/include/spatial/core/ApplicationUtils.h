#pragma once

#include <spatial/core/Application.h>
#include <spatial/common/EventQueue.h>
#include <spatial/common/EventQueueUtils.h>
#include <boost/tti/has_member_function.hpp>

namespace spatial
{

BOOST_TTI_HAS_MEMBER_FUNCTION(onStart);

BOOST_TTI_HAS_MEMBER_FUNCTION(onStartFrame);

BOOST_TTI_HAS_MEMBER_FUNCTION(onUpdateFrame);

BOOST_TTI_HAS_MEMBER_FUNCTION(onEndFrame);

BOOST_TTI_HAS_MEMBER_FUNCTION(onEndGuiFrame);

BOOST_TTI_HAS_MEMBER_FUNCTION(onDrawGui);

BOOST_TTI_HAS_MEMBER_FUNCTION(onFinish);

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

template <typename Listener>
void connect(Application& app, Listener& listener)
{
	if constexpr (has_on_start_v<Listener>)
		app.getStartSignal().connect<&Listener::onStart>(listener);

	if constexpr (has_on_start_frame_v<Listener>)
		app.getStartFrameSignal().connect<&Listener::onStartFrame, Listener>(listener);

	if constexpr (has_on_update_frame_v<Listener>)
		app.getUpdateFrameSignal().connect<&Listener::onUpdateFrame, Listener>(listener);

	if constexpr (has_on_end_frame_v<Listener>)
		app.getEndFrameSignal().connect<&Listener::onEndFrame, Listener>(listener);

	if constexpr (has_on_end_gui_frame_v<Listener>)
		app.getEndGuiFrameSignal().connect<&Listener::onEndGuiFrame, Listener>(listener);

	if constexpr (has_on_draw_gui_v<Listener>)
		app.getDrawGuiSignal().connect<&Listener::onDrawGui>(listener);

	if constexpr (has_on_finish_v<Listener>)
		app.getFinishSignal().connect<&Listener::onFinish>(listener);

	connect<WindowResizedEvent>(app.getEventQueue(), listener);
	connect<WindowClosedEvent>(app.getEventQueue(), listener);
	connect<KeyEvent>(app.getEventQueue(), listener);
	connect<TextEvent>(app.getEventQueue(), listener);
	connect<MouseMovedEvent>(app.getEventQueue(), listener);
	connect<MouseScrolledEvent>(app.getEventQueue(), listener);
}

template <typename... Listeners>
void connect(Application& app, Listeners&... listener)
{
	(connect(app, listener), ...);
}

template <typename Listener>
void disconnect(Application& app, Listener& listener)
{
	if constexpr (has_on_start_v<Listener>)
		app.getStartSignal().disconnect<&Listener::onStart>(listener);

	if constexpr (has_on_start_frame_v<Listener>)
		app.getStartFrameSignal().disconnect<&Listener::onStartFrame, Listener>(listener);

	if constexpr (has_on_update_frame_v<Listener>)
		app.getUpdateFrameSignal().disconnect<&Listener::onUpdateFrame, Listener>(listener);

	if constexpr (has_on_end_frame_v<Listener>)
		app.getEndFrameSignal().disconnect<&Listener::onEndFrame, Listener>(listener);

	if constexpr (has_on_end_gui_frame_v<Listener>)
		app.getEndGuiFrameSignal().disconnect<&Listener::onEndGuiFrame, Listener>(listener);

	if constexpr (has_on_draw_gui_v<Listener>)
		app.getDrawGuiSignal().disconnect<&Listener::onDrawGui>(listener);

	if constexpr (has_on_finish_v<Listener>)
		app.getFinishSignal().disconnect<&Listener::onFinish>(listener);

	disconnect<WindowResizedEvent>(app.getEventQueue(), listener);
	disconnect<WindowClosedEvent>(app.getEventQueue(), listener);
	disconnect<KeyEvent>(app.getEventQueue(), listener);
	disconnect<TextEvent>(app.getEventQueue(), listener);
	disconnect<MouseMovedEvent>(app.getEventQueue(), listener);
	disconnect<MouseScrolledEvent>(app.getEventQueue(), listener);
}

template <typename... Listeners>
void disconnect(Application& app, Listeners&... listener)
{
	(disconnect(app, listener), ...);
}

} // namespace spatial
