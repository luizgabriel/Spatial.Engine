#pragma once

#include <boost/tti/has_member_function.hpp>
#include <spatial/common/EventQueue.h>
#include <spatial/core/Application.h>

namespace spatial
{

namespace detail
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
constexpr bool has_on_finish_v = has_member_function_onFinish<T, void>::value;

} // namespace detail

template <typename Listener>
void connect(Application& app, Listener& listener)
{
	if constexpr (detail::has_on_start_v<Listener>)
		app.getStartSignal().connect<&Listener::onStart>(listener);

	if constexpr (detail::has_on_start_frame_v<Listener>)
		app.getStartFrameSignal().connect<&Listener::onStartFrame, Listener>(listener);

	if constexpr (detail::has_on_update_frame_v<Listener>)
		app.getUpdateFrameSignal().connect<&Listener::onUpdateFrame, Listener>(listener);

	if constexpr (detail::has_on_end_frame_v<Listener>)
		app.getEndFrameSignal().connect<&Listener::onEndFrame, Listener>(listener);

	if constexpr (detail::has_on_finish_v<Listener>)
		app.getFinishSignal().connect<&Listener::onFinish>(listener);
}

template <typename Listener>
void disconnect(Application& app, Listener& listener)
{
	if constexpr (detail::has_on_start_v<Listener>)
		app.getStartSignal().disconnect<&Listener::onStart>(listener);

	if constexpr (detail::has_on_start_frame_v<Listener>)
		app.getStartFrameSignal().disconnect<&Listener::onStartFrame, Listener>(listener);

	if constexpr (detail::has_on_update_frame_v<Listener>)
		app.getUpdateFrameSignal().disconnect<&Listener::onUpdateFrame, Listener>(listener);

	if constexpr (detail::has_on_end_frame_v<Listener>)
		app.getEndFrameSignal().disconnect<&Listener::onEndFrame, Listener>(listener);

	if constexpr (detail::has_on_finish_v<Listener>)
		app.getFinishSignal().disconnect<&Listener::onFinish>(listener);
}

template <typename Listener>
Application& operator>>(Application& app, Listener& listener)
{
	connect(app, listener);
	return app;
}

} // namespace spatial
