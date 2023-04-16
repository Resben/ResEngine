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
		MouseMoved(Math::vec2 pos)
		: m_pos(pos) {}

		Math::vec2 GetPos() const { return m_pos; }
		AE_EVENT_META(Window, MouseMoved)

	private:
		Math::vec2 m_pos;
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
		MouseScrolled(Math::vec2 scroll)
			: m_scroll(scroll) {}

		Math::vec2 GetScroll() { return m_scroll; }
		AE_EVENT_META(Window, MouseScrolled)

	private:
		Math::vec2 m_scroll;
	};
}
