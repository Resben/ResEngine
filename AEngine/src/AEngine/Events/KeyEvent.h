#include "Platform/Windows/WindowsKeys.h"
#include "Event.h"

namespace AEngine
{
	class KeyPressedEvent : public Event
	{
	public:
		KeyPressedEvent(const AEKey key)
		: m_key(key) {}

		AEKey GetKey() const { return m_key; }

		EventType GetType() const override { return  AEngine::EventType::KeyPressed; }
		const char* GetName() const override { return "Key was pressed"; }


	private:
		AEKey m_key;
	};

	class KeyReleasedEvent : public Event
	{
	public:
		KeyReleasedEvent(const AEKey key)
		: m_key(key) {}

		AEKey GetKey() const { return m_key; }

		EventType GetType() const override { return AEngine::EventType::KeyReleased; }
		const char* GetName() const override { return "Key was released"; }

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
		const char* GetName() const override { return "Key was typed"; }

	private:
		AEKey m_key;
	};
}