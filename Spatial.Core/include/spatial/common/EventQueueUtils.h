#pragma once

#include <spatial/common/EventQueue.h>
#include <boost/tti/has_member_function.hpp>

namespace spatial
{

BOOST_TTI_HAS_MEMBER_FUNCTION(onEvent);

template <typename T, typename E>
constexpr bool has_on_event_v = has_member_function_onEvent<T, void, boost::mpl::vector<const E&>>::value;

template <typename Event, typename Handler>
void connect(EventQueue& queue, Handler* instance)
{
	if constexpr (has_on_event_v<Handler, Event>)
		queue.template connect<Event>(instance);
}

template <typename Event, typename Handler>
void disconnect(EventQueue& queue, Handler* instance)
{
	if constexpr (has_on_event_v<Handler, Event>)
		queue.template disconnect<Event>(instance);
}

} // namespace spatial