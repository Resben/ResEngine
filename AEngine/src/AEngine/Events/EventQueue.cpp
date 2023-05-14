/**
 * @file
 * @author Christien Alden (34119981)
**/
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
		: m_windowEvents(), m_gameEvents()
	{

	}

	void EventQueue::Clear(EventCategory type)
	{
		switch (type)
		{
		case EventCategory::Window:
			m_windowEvents.clear();
			break;
		case EventCategory::Game:
			m_gameEvents.clear();
			break;
		}
	}

	void EventQueue::PushEvent(std::unique_ptr<Event> event)
	{
		switch (event->GetCategory())
		{
		case EventCategory::Window:
			m_windowEvents.push_back(std::move(event));
			break;
		case EventCategory::Game:
			m_gameEvents.push_back(std::move(event));
			break;
		}
	}

	EventQueue::queue* EventQueue::GetEventQueue(EventCategory type)
	{
		switch (type)
		{
		case EventCategory::Window:
			return &m_windowEvents;
		case EventCategory::Game:
			return &m_gameEvents;
		default:
			AE_LOG_ERROR("EventQueue::GetEventQueue::Invalid_EventCategory -> {}", static_cast<int>(type));
			return nullptr;
		}
	}
}
