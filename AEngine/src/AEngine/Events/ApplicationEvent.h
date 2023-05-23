/**
 * \file
 * \author Christien Alden (34119981)
 * \author Ben Hawkins (34112619)
*/
#pragma once
#include "AEngine/Core/Logger.h"
#include "Event.h"

namespace AEngine
{
		/**
		 * \class WindowResized
		 * \brief Created when the application window is resized
		**/
	class WindowResized : public Event
	{
	public:
			/**
			 * \param[in] width New window width
			 * \param[in] height New window height
			*/
		WindowResized(unsigned int width, unsigned int height)
			: m_width{ width }, m_height{ height } {}

			/**
			 * \brief Get the new window width
			 * \return New window width
			*/
		unsigned int GetWidth() const { return m_width; }
			/**
			 * \brief Get the new window height
			 * \return New window height
			*/
		unsigned int GetHeight() const { return m_height; }
		AE_EVENT_META(Window, WindowResized)

	private:
		unsigned int m_width, m_height;
	};

		/**
		 * \class WindowClosed
		 * \brief Created when the application window is closed
		*/
	class WindowClosed : public Event
	{
	public:
		WindowClosed() = default;

		AE_EVENT_META(Window, WindowClosed)
	};
}
