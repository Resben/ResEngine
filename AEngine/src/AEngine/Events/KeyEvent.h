#pragma once
#include "AEngine/Core/KeyCodes.h"
#include "Event.h"

namespace AEngine
{
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