#pragma once
#include "Event.h"

namespace AEngine
{
	class WindowResized : public Event
	{
	public:
		WindowResized(unsigned int width, unsigned int height)
			: m_width(width), m_height(height) {}

		unsigned int GetWidth() const { return m_width; }
		unsigned int GetHeight() const { return m_height; }
		EventType GetType() const override { return  EventType::WindowResized; }
		const char* GetName() const override { return "WindowResized"; }
		static EventType GetStaticType() { return EventType::WindowResized; }

	private:
		unsigned int m_width, m_height;
	};

	class WindowClosed : public Event
	{
	public:
		WindowClosed() = default;

		EventType GetType() const override { return  EventType::WindowClosed; }
		const char* GetName() const override { return "WindowClosed"; }
		static EventType GetStaticType() { return EventType::WindowClosed; }
	};
}