#include "EventQueue.h"

namespace AEngine
{
	EventQueue* EventQueue::s_instance = nullptr;

	EventQueue& EventQueue::Instance()
	{
		if (!s_instance)
		{
			s_instance = new EventQueue();
		}

		return *s_instance;
	}

	EventQueue::EventQueue()
		: m_queue()
	{

	}

	void EventQueue::Clear()
	{
		m_queue.clear();
	}

	void EventQueue::PushEvent(Event* event)
	{
		m_queue.push_back(event);
	}

	std::list<Event*>::iterator EventQueue::begin()
	{
		return m_queue.begin();
	}

	std::list<Event*>::iterator EventQueue::end()
	{
		return m_queue.end();
	}
}
