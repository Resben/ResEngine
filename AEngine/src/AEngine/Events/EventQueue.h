/**
 * @file
 * @author Christien Alden (34119981)
**/
#pragma once
#include <list>
#include <functional>
#include "Event.h"
#define AE_EVENT_FN(fn) std::bind(fn, this, std::placeholders::_1)

namespace AEngine
{
		/**
		 * @class EventQueue
		 * @brief A service to queue and dispatch events
		 *
		 * Events can be pushed to the queue from anywhere in the codebase,
		 * it's EventCategory will determine which internal structure it is
		 * placed into.
		 *
		 * When the engine (or client) needs to query the EventQueue they
		 * can use the EventDispatcher with a given EventType to retrieve
		 * the event (if any) from the appropriate structure.
		**/
	class EventQueue
	{
	public:
			/**
			 * @brief Returns the instance of the EventQueue
			 * @return The EventQueue instance
			**/
		static EventQueue& Instance();
			/**
			 * @brief Pushes an event to be processed later
			 * @param[in] event to be queued
			 * @retval void
			 *
			 * The class will determine which internal structure the
			 * event will be placed in.
			**/
		void PushEvent(Event* event);
			/**
			 * @brief Clears an entire internal queue
			 * @param[in] type the category (queue) to clear
			 * @retval void
			**/
		void Clear(EventCategory type = EventCategory::Window);
		
	private:
		using eventList = std::list<Event*>;
		static EventQueue* s_instance;
		eventList m_windowEvents;
		eventList m_gameEvents;

			/**
			 * @brief Returns the internal event queue based a EventCategory
			 * @param[in] type the category (queue) to fetch
			 * @return the internal queue
			 *
			 * This is designed to be called by EventDispatcher
			**/
		eventList& GetEventQueue(EventCategory type);

		EventQueue();
		friend class EventDispatcher;
	};

		/**
		 * @class EventDispatcher
		 * @brief Gives the ability for classes within the engine to retrieve
		 * events from the EventQueue
		**/
	class EventDispatcher
	{
	public:
		EventDispatcher() = default;

			/**
			 * @brief Dispatches all events of the given type and calls the event handling function
			 * @param[in] func Event handling function to call
			 * @retval void
			 * @note If \p func returns true, the corresponding event will be removed from the queue;
			 * if not, the event will be able to be processed further down the line.
			 *
			 * The method will use the template parameter to get the correct internal queue and
			 * dynamically typecast the Event to match the handling function.
			**/
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
