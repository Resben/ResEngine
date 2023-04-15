#pragma once

namespace AEngine
{
	enum class EventType
	{
		WindowClosed, WindowResized,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, 
		MouseMoved, MouseScrolled
	};

	enum class EventCategory
	{
		Window, Game
	};
	
	class Event
	{
	public:
		virtual ~Event() = default;
		bool handled = false;
		virtual const char* GetName() const = 0;
		virtual EventType GetType() const = 0;
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
