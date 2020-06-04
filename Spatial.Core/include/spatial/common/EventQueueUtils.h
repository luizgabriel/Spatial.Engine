#pragma once

#include <spatial/common/EventQueue.h>
#include <boost/tti/has_member_function.hpp>

namespace spatial
{

BOOST_TTI_HAS_MEMBER_FUNCTION(onEvent);

template <typename T, typename E>
constexpr bool has_on_event_v = has_member_function_onEvent<T, void, boost::mpl::vector<const E&>>::value;

template <typename Event, typename Listener>
void connect(EventQueue& queue, Listener& listener)
{
	if constexpr (has_on_event_v<Listener, Event>)
		queue.connect<Event, nullptr, Listener>(listener);
}

template <typename Event, typename Listener>
void disconnect(EventQueue& queue, Listener& listener)
{
	if constexpr (has_on_event_v<Listener, Event>)
		queue.disconnect<Event>(listener);
}

} // namespace spatial