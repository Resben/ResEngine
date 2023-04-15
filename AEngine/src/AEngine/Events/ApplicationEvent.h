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
		AE_EVENT_META(Window, WindowResized)

	private:
		unsigned int m_width, m_height;
	};

	class WindowClosed : public Event
	{
	public:
		WindowClosed() = default;

		AE_EVENT_META(Window, WindowClosed)
	};
}
