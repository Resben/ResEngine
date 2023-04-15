#pragma once
#include <list>
#include <functional>
#include "Event.h"
#define AE_EVENT_FN(fn) std::bind(fn, this, std::placeholders::_1)

namespace AEngine
{
	class EventQueue
	{
	public:
		static EventQueue& Instance();
		void PushEvent(Event* event);	
		void Clear(EventCategory type = EventCategory::Window);
		
	private:
		using eventList = std::list<Event*>;
		static EventQueue* s_instance;
		eventList m_windowEvents;
		eventList m_gameEvents;

		eventList& GetEventQueue(EventCategory type);

		EventQueue();
		friend class EventDispatcher;
	};

	class EventDispatcher
	{
	public:
		EventDispatcher() = default;

		template <typename T>
		void Dispatch(std::function<bool(T&)> func)
		{
			using eventList = std::list<Event*>;
			eventList& events = EventQueue::Instance().GetEventQueue(T::GetStaticCategory());
			eventList::iterator it;

			for (it = events.begin(); it != events.end(); ++it)
			{
				// only process the right event
				if (T::GetStaticType() != (*it)->GetType())
				{
					continue;
				}
				
				// process event
				bool handled = func(dynamic_cast<T&>(**it));
				if (handled)
				{
					// clean-up event
					delete (*it);
					it = events.erase(it);
					if (it == events.end())
					{
						break;
					}
				}
			}
		};
	};
}
