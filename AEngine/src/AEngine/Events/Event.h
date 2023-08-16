/**
 * \file
 * \author Christien Alden (34119981)
 * \author Ben Hawkins (34112619)
**/
#pragma once

namespace AEngine
{
		/**
		 * \enum EventType
		 * \brief Used to dynamically cast a base event to the correct
		 * specialisation when dispatching events
		**/
	enum class EventType
	{
		WindowClosed, WindowResized,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
	};

		/**
		 * \enum EventCategory
		 * \brief Used to determine which event queue the event is placed in
		**/
	enum class EventCategory
	{
		Window, Key, Mouse
	};

		/**
		 * \class Event
		 * \brief Provides the base messaging system of the engine
		**/
	class Event
	{
	public:
		virtual ~Event() = default;
			/**
			 * \brief Returns the Event name
			 * \return The a stringified name of the class
			**/
		virtual const char* GetName() const = 0;
			/**
			 * \brief Returns the EventType
			 * \return The enum class for the Event type
			**/
		virtual EventType GetType() const = 0;
			/**
			 * \brief Returns the EventCategory
			 * \return The enum class for the Event category
			**/
		virtual EventCategory GetCategory() const = 0;
	};

	// AE_EVENT_META is used to generate the generic overrides for the subclasses
	// these methods are used in the event system to dynamically cast the events
	// to the correct type during dispatch and allocate them to the correct queue
	// upon pushing to the EventQueue
#define AE_EVENT_META(category, type) \
	const char* GetName() const override { return #type; } \
	static EventType GetStaticType() { return EventType::type; } \
	static EventCategory GetStaticCategory() { return EventCategory::category; } \
	EventType GetType() const override { return GetStaticType(); } \
	EventCategory GetCategory() const override { return GetStaticCategory(); }
}
