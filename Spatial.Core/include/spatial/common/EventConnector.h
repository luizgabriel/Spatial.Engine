#pragma once

#include <spatial/common/EventQueue.h>

namespace spatial::common
{

template <typename Event, typename Handler>
class EventConnector
{
private:
	common::EventQueue* m_queue;
	Handler* m_handler;

public:
	EventConnector(common::EventQueue& queue, Handler* handler) : m_queue{&queue}, m_handler{handler}
	{
		m_queue->template connect<Event>(m_handler);
	}

	~EventConnector() { m_queue->template disconnect<Event>(m_handler); }

	EventConnector(const EventConnector& other) = delete;
	EventConnector& operator=(const EventConnector& other) = delete;
};

} // namespace spatial::common