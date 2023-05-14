/**
 * @file
 * @author Christien Alden (34119981)
**/
#pragma once
#include "AEngine/Core/Logger.h"
#include <list>
#include <functional>
#include <memory>
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
		using queue = std::list<UniquePtr<Event>>;
	
	public:
			/**
			 * @brief Returns the instance of the EventQueue
			 * @return The EventQueue instance
			**/
		static EventQueue& Instance();
			/**
			 * @brief Pushes an event to be processed later
			 * @param[in] event to be queued
			 *
			 * @details
			 * The class will determine which internal structure the
			 * event will be placed in.
			**/
		void PushEvent(UniquePtr<Event> event);
			/**
			 * @brief Clears an entire internal queue
			 * @param[in] type the category (queue) to clear
			 * @retval void
			**/
		void Clear(EventCategory type = EventCategory::Window);
		
	private:
		static EventQueue* s_instance;
		queue m_windowEvents;
		queue m_gameEvents;

			/**
			 * @brief Returns the internal event queue based a EventCategory
			 * @param[in] type the category (queue) to fetch
			 * @return the internal queue
			 *
			 * This is designed to be called by EventDispatcher
			**/
		queue* GetEventQueue(EventCategory type);

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
			 * @tparam EventType The type of event to dispatch
			 * @param[in] func Event handling function to call
			 * 
			 * @note If \p func returns true, the corresponding event will be removed from the queue;
			 * if not, the event will be able to be processed further down the line.
			 *
			 * @details
			 * The method will use the template parameter to get the correct internal queue and
			 * dynamically typecast the Event to match the handling function.
			**/
		template <typename EventType>
		void Dispatch(std::function<bool(EventType&)> func)
		{
			// get event queue for T
			EventQueue::queue* events = EventQueue::Instance().GetEventQueue(EventType::GetStaticCategory());
			if (events == nullptr)
			{
				// error getting queue
				AE_LOG_FATAL("EventDispatcher::Dispatch::NoQueue");
			}

			EventQueue::queue::iterator it;

			for (it = events->begin(); it != events->end(); ++it)
			{
				// only process the right event
				if (EventType::GetStaticType() != (*it)->GetType())
				{
					continue;
				}
				
				// process event
				bool handled = func(dynamic_cast<EventType&>(**it));
				if (handled)
				{
					// clean-up event
					it = events->erase(it);
					if (it == events->end())
					{
						break;
					}
				}
			}
		};
	};
}
