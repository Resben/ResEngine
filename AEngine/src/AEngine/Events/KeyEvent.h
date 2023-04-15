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
		EventCategory GetCategory() const override { return EventCategory::Window; }
		static EventCategory GetStaticCategory() { return EventCategory::Window; }
		static EventType GetStaticType() { return EventType::KeyPressed; }

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
		EventCategory GetCategory() const override { return EventCategory::Window; }
		static EventCategory GetStaticCategory() { return EventCategory::Window; }
		static EventType GetStaticType() { return EventType::KeyReleased; }

	private:
		AEKey m_key;
	};

	class KeyTyped : public Event
	{
	public:
		KeyTyped(unsigned int key)
		: m_key(key) {}

		unsigned int GetKey() const { return m_key; }
		EventType GetType() const override { return AEngine::EventType::KeyTyped; }
		const char* GetName() const override { return "KeyTyped"; }
		EventCategory GetCategory() const override { return EventCategory::Window; }
		static EventCategory GetStaticCategory() { return EventCategory::Window; }
		static EventType GetStaticType() { return EventType::KeyTyped; }

	private:
		unsigned int m_key;
	};
}