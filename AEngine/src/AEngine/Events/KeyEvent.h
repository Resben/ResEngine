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
		EventType GetType() const override { return  EventType::KeyPressed; }
		const char* GetName() const override { return "KeyPressed"; }
	private:
		AEKey m_key;
	};

	class KeyReleased : public Event
	{
	public:
		KeyReleased(const AEKey key)
		: m_key(key) {}

		AEKey GetKey() const { return m_key; }
		EventType GetType() const override { return EventType::KeyReleased; }
		const char* GetName() const override { return "KeyReleased"; }

	private:
		AEKey m_key;
	};

	class KeyTyped : public Event
	{
	public:
		KeyTyped(const AEKey key)
		: m_key(key) {}

		AEKey GetKey() const { return m_key; }
		EventType GetType() const override { return AEngine::EventType::KeyTyped; }
		const char* GetName() const override { return "KeyTyped"; }

	private:
		AEKey m_key;
	};
}