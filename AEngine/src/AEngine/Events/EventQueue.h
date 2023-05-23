/**
 * \file
 * \author Christien Alden (34119981)
 * \brief EventQueue and EventDispatcher classes
*/
#pragma once
#include "AEngine/Core/Logger.h"
#include "Event.h"
#include <functional>
#include <list>
#include <memory>
#define AE_EVENT_FN(fn) std::bind(fn, this, std::placeholders::_1)

namespace AEngine
{
		/**
		 * \class EventQueue
		 * \brief A service to queue and dispatch events
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
		using Queue = std::list<UniquePtr<Event>>;

	public:
			/**
			 * \brief Returns the instance of the EventQueue
			 * \return The EventQueue instance
			**/
		static EventQueue& Instance();
			/**
			 * \brief Pushes an event to be processed later
			 * \param[in] event to be queued
			 *
			 * \details
			 * The class will determine which internal structure the
			 * event will be placed in.
			**/
		void PushEvent(UniquePtr<Event> event);
			/**
			 * \brief Clears an entire internal Queue
			 * \param[in] type the category (Queue) to clear
			**/
		void Clear(EventCategory type = EventCategory::Window);

	private:
			/**
			 * \brief The instance of the EventQueue
			*/
		static EventQueue* s_instance;
			/**
			 * \brief Queue for window events
			*/
		Queue m_windowEvents;
			/**
			 * \brief Queue for game events
			 * \note Currently unused
			*/
		Queue m_gameEvents;

			/**
			 * \brief Returns the internal event Queue based a EventCategory
			 * \param[in] type the category (Queue) to fetch
			 * \return the internal Queue
			 *
			 * This is designed to be called by EventDispatcher
			**/
		Queue* GetEventQueue(EventCategory type);

		EventQueue();
		friend class EventDispatcher;
	};

		/**
		 * \class EventDispatcher
		 * \brief Gives the ability for classes within the engine to retrieve
		 * events from the EventQueue
		**/
	class EventDispatcher
	{
	public:
		EventDispatcher() = default;

			/**
			 * \brief Dispatches all events of the given type and calls the event handling function
			 * \tparam EventType The type of event to dispatch
			 * \param[in] func Event handling function to call
			 *
			 * \note If \p func returns true, the corresponding event will be removed from the queue;
			 * if not, the event will be able to be processed further down the line.
			 *
			 * \details
			 * The method will use the template parameter to get the correct internal Queue and
			 * dynamically typecast the Event to match the handling function.
			**/
		template <typename EventType>
		void Dispatch(std::function<bool(EventType&)> func)
		{
			// get event queue for T
			EventQueue::Queue* events = EventQueue::Instance().GetEventQueue(EventType::GetStaticCategory());
			if (events == nullptr)
			{
				// error getting queue
				AE_LOG_FATAL("EventDispatcher::Dispatch::NoQueue");
			}

			EventQueue::Queue::iterator it;
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
