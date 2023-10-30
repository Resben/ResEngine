/**
 * \file
 * \author Christien Alden (34119981)
 * \author Ben Hawkins (34112619)
*/
#pragma once
#include "AEngine/Input/KeyCodes.h"
#include "AEngine/Math/Math.h"
#include "Event.h"

namespace AEngine
{
		/**
		 * \class MouseReleased
		 * \brief Created when a mouse button has been released
		*/
	class MouseReleased : public Event
	{
	public:
			/**
			 * \param[in] button The mouse button that was released
			*/
		MouseReleased(const AEMouse button)
			: m_mouseButton{ button } {}

			/**
			 * \brief Get the mouse button that was released
			 * \return The mouse button that was released
			*/
		AEMouse GetButton() { return m_mouseButton; }
		AE_EVENT_META(Mouse, MouseReleased)

	private:
		AEMouse m_mouseButton;
	};

		/**
		 * \class MousePressed
		 * \brief Created when a mouse button has been pressed
		*/
	class MousePressed : public Event
	{
	public:
			/**
			 * \param[in] button The mouse button that was pressed
			*/
		MousePressed(const AEMouse button)
			: m_mouseButton{ button } {}

			/**
			 * \brief Get the mouse button that was pressed
			 * \return The mouse button that was pressed
			*/
		AEMouse GetButton() { return m_mouseButton; }
		AE_EVENT_META(Mouse, MousePressed)

	private:
		AEMouse m_mouseButton;
	};

	class MouseRepeated : public Event
	{
	public:
		MouseRepeated(const AEMouse button)
			: m_mouseButton{ button } {}

		AEMouse GetButton() { return m_mouseButton; }
		AE_EVENT_META(Mouse, MouseRepeated)

	private:
		AEMouse m_mouseButton;
	};

	class MouseMoved : public Event
	{
	public:
			/**
			 * \param[in] pos The new cursor position
			*/
		MouseMoved(Math::vec2 pos, Math::vec2 delta)
			: m_pos{ pos }, m_delta { delta } {}

			/**
			 * \brief Get the new cursor position
			 * \return The new cursor position
			*/
		Math::vec2 GetPos() const { return m_pos; }
		Math::vec2 GetDelta() const { return m_delta; }
		AE_EVENT_META(Mouse, MouseMoved)

	private:
		Math::vec2 m_pos;
		Math::vec2 m_delta;
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
		AE_EVENT_META(Mouse, MouseScrolled)

	private:
		Math::vec2 m_scroll;
	};
}
