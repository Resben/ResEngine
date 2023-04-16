/**
 * @file
 * @author Christien Alden (34119981)
 * @author Ben Hawkins (34112619)
**/
#pragma once
#include "Event.h"
#include "AEngine/Core/KeyCodes.h"

namespace AEngine
{
		/**
		 * @class MouseMoved
		 * @brief Created when the cursor is moved in the application window
		**/
	class MouseMoved : public Event
	{
	public:
		MouseMoved(double xpos, double ypos)
		: m_xPos(xpos), m_yPos(ypos) {}

		double GetX() const { return m_xPos; }
		double GetY() const { return m_yPos; }
		AE_EVENT_META(Window, MouseMoved)

	private:
		double m_xPos, m_yPos;
	};

		/**
		 * @class MouseButtonPressed
		 * @brief Created when a mouse button has been pressed
		**/
	class MouseButtonPressed : public Event
	{
	public:
		MouseButtonPressed(const AEMouse button)
			: m_mouseButton(button) {}

		AEMouse GetButton() { return m_mouseButton; }
		AE_EVENT_META(Window, MouseButtonPressed)

	private:
		AEMouse m_mouseButton;
	};

		/**
		 * @class MouseButtonReleased
		 * @brief Created when a mouse button has been released
		**/
	class MouseButtonReleased : public Event
	{
	public:
		MouseButtonReleased(const AEMouse button)
			: m_mouseButton(button) {}

		AEMouse GetButton() { return m_mouseButton; }
		AE_EVENT_META(Window, MouseButtonReleased)

	private:
		AEMouse m_mouseButton;
	};

		/**
		 * @class MouseScrolled
		 * @brief Created when the mouse wheel is scrolled
		**/
	class MouseScrolled : public Event
	{
	public:
		MouseScrolled(double x, double y)
			: m_scrollX(x), m_scrollY(y) {}

		double GetX() { return m_scrollX; }
		double GetY() { return m_scrollY; }
		AE_EVENT_META(Window, MouseScrolled)

	private:
		double m_scrollX, m_scrollY;
	};
}
