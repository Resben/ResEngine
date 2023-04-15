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

	class Event
	{
	public:
		virtual ~Event() = default;
		bool handled = false;
		virtual const char* GetName() const = 0;
		virtual EventType GetType() const = 0;
	};
}

