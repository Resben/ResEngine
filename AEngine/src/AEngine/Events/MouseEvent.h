/**
 * \file
 * \author Christien Alden (34119981)
 * \author Ben Hawkins (34112619)
*/
#pragma once
#include "AEngine/Input/KeyCodes.h"
#include "Event.h"

namespace AEngine
{
		/**
		 * \class MouseMoved
		 * \brief Created when the cursor is moved in the application window
		*/
	class MouseMoved : public Event
	{
	public:
			/**
			 * \param[in] pos The new cursor position
			*/
		MouseMoved(Math::vec2 pos)
			: m_pos{ pos } {}

			/**
			 * \brief Get the new cursor position
			 * \return The new cursor position
			*/
		Math::vec2 GetPos() const { return m_pos; }
		AE_EVENT_META(Window, MouseMoved)

	private:
		Math::vec2 m_pos;
	};

		/**
		 * \class MouseButtonPressed
		 * \brief Created when a mouse button has been pressed
		*/
	class MouseButtonPressed : public Event
	{
	public:
			/**
			 * \param[in] button The mouse button that was pressed
			*/
		MouseButtonPressed(const AEMouse button)
			: m_mouseButton{ button } {}

			/**
			 * \brief Get the mouse button that was pressed
			 * \return The mouse button that was pressed
			*/
		AEMouse GetButton() { return m_mouseButton; }
		AE_EVENT_META(Window, MouseButtonPressed)

	private:
		AEMouse m_mouseButton;
	};

		/**
		 * \class MouseButtonReleased
		 * \brief Created when a mouse button has been released
		*/
	class MouseButtonReleased : public Event
	{
	public:
			/**
			 * \param[in] button The mouse button that was released
			*/
		MouseButtonReleased(const AEMouse button)
			: m_mouseButton{ button } {}

			/**
			 * \brief Get the mouse button that was released
			 * \return The mouse button that was released
			*/
		AEMouse GetButton() { return m_mouseButton; }
		AE_EVENT_META(Window, MouseButtonReleased)

	private:
		AEMouse m_mouseButton;
	};

		/**
		 * \class MouseScrolled
		 * \brief Created when the mouse wheel is scrolled
		*/
	class MouseScrolled : public Event
	{
	public:
			/**
			 * \param[in] scroll The amount the mouse wheel was scrolled
			*/
		MouseScrolled(Math::vec2 scroll)
			: m_scroll{ scroll } {}

			/**
			 * \brief Get the amount the mouse wheel was scrolled
			 * \return The amount the mouse wheel was scrolled
			*/
		Math::vec2 GetScroll() { return m_scroll; }
		AE_EVENT_META(Window, MouseScrolled)

	private:
		Math::vec2 m_scroll;
	};
}
