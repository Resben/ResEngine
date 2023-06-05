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
		 * \class KeyPressed
		 * \brief Created when a key is pressed
		*/
	class KeyPressed : public Event
	{
	public:
			/**
			 * \param[in] key The key that was pressed
			 */
		KeyPressed(const AEKey key)
			: m_key{ key } {}

			/**
			 * \brief Get the key that was pressed
			 * \return The key that was pressed
			 */
		AEKey GetKey() const { return m_key; }
		AE_EVENT_META(Window, KeyPressed)

	private:
		AEKey m_key;
	};

		/**
		 * \class KeyReleased
		 * \brief Created when a key is released
		*/
	class KeyReleased : public Event
	{
	public:
			/**
			 * \param[in] key The key that was released
			 */
		KeyReleased(const AEKey key)
			: m_key{ key } {}

			/**
			 * \brief Get the key that was released
			 * \return The key that was released
			 */
		AEKey GetKey() const { return m_key; }
		AE_EVENT_META(Window, KeyReleased)

	private:
		AEKey m_key;
	};

		/**
		 * \class KeyTyped
		 * \brief Created when a key has been pressed and contains the character
		 * code to display printable characters, i.e., text.
		*/
	class KeyTyped : public Event
	{
	public:
			/**
			 * \param[in] key The key that was typed
			 */
		KeyTyped(unsigned int key)
			: m_key{ key } {}

			/**
			 * \brief Get the key that was typed
			 * \return The key that was typed
			 */
		unsigned int GetKey() const { return m_key; }
		AE_EVENT_META(Window, KeyTyped)

	private:
		unsigned int m_key;
	};
}
