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
		 * @class KeyPressed
		 * @brief Created when a key is pressed
		**/
	class KeyPressed : public Event
	{
	public:
		KeyPressed(const AEKey key)
		: m_key(key) {}

		AEKey GetKey() const { return m_key; }
		AE_EVENT_META(Window, KeyPressed)

	private:
		AEKey m_key;
	};

		/**
		 * @class KeyReleased
		 * @brief Created when a key is released
		**/
	class KeyReleased : public Event
	{
	public:
		KeyReleased(const AEKey key)
		: m_key(key) {}

		AEKey GetKey() const { return m_key; }
		AE_EVENT_META(Window, KeyReleased)

	private:
		AEKey m_key;
	};

		/**
		 * @class KeyTyped
		 * @brief Created when a key has been pressed and contains the character
		 * code to display printable characters, i.e., text.
		**/
	class KeyTyped : public Event
	{
	public:
		KeyTyped(unsigned int key)
		: m_key(key) {}

		unsigned int GetKey() const { return m_key; }
		AE_EVENT_META(Window, KeyTyped)
	
	private:
		unsigned int m_key;
	};
}
