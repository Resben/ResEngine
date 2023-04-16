/**
 * @file
 * @author Christien Alden (34119981)
 * @author Ben Hawkins (34112619)
**/
#pragma once
#include "Event.h"

namespace AEngine
{
		/**
		 * @class WindowResized
		 * @brief Created when the application window is resized
		**/
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

		/**
		 * @class WindowClosed
		 * @brief Created when the application window is closed
		**/
	class WindowClosed : public Event
	{
	public:
		WindowClosed() = default;

		AE_EVENT_META(Window, WindowClosed)
	};
}
