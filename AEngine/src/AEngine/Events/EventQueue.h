#pragma once
#include <list>
#include <functional>
#include "Event.h"

namespace AEngine
{
	class EventQueue
	{
	public:
		static EventQueue& Instance();
		void PushEvent(Event* event);	
		void Clear();
		
	private:
		static EventQueue* s_instance;

		std::list<Event*>::iterator begin();
		std::list<Event*>::iterator end();
		std::list<Event*> m_queue;
		
		friend class EventDispatcher;
	};

	class EventDispatcher
	{
	public:
		using eventHandler = std::function<bool(Event&)>;
		EventDispatcher(eventHandler func);
		void Dispatch(EventType type);
		void UpdateHandler(eventHandler func);

	private:
		eventHandler m_handler;
	};
}
