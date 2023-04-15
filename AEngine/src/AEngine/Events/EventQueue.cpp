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

	// ----------------------------------------------------------------------------

	using eventHandler = std::function<bool(Event&)>;
	EventDispatcher::EventDispatcher(eventHandler func)
		: m_handler(func)
	{

	}

	void EventDispatcher::Dispatch(EventType type)
	{
		std::list<Event*> events = EventQueue::Instance().m_queue;
		std::list<Event*>::iterator it;
		for (it = events.begin(); it != events.end(); ++it)
		{
			if (type == (*it)->GetType())
			{
				bool handled = m_handler(**it);
				if (handled)
				{
					events.erase(it);
				}
			}
		}
	}

	void EventDispatcher::UpdateHandler(eventHandler func)
	{
		m_handler = func;
	}
}
