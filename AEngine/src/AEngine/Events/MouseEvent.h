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
		MouseMoved(const float x, const float y)
		: m_mouseX(x), m_mouseY(y) {}

		float GetX() const { return m_mouseX; }
		float GetY() const { return m_mouseY; }
		AE_EVENT_META(Window, MouseMoved)

	private:
		float m_mouseX, m_mouseY;
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
		MouseScrolled(const float x, const float y)
			: m_scrollX(x), m_scrollY(y) {}

		float GetScrollX() { return m_scrollX; }
		float GetScrollY() { return m_scrollY; }
		AE_EVENT_META(Window, MouseScrolled)

	private:
		float m_scrollX, m_scrollY;
	};
}
