#include "Platform/Windows/WindowsKeys.h"
#include "Event.h"

namespace AEngine
{
	class MouseMovedEvent : public Event
	{
	public:
		MouseMovedEvent(const float x, const float y)
		: m_mouseX(x), m_mouseY(y) {}

		float GetX() const { return m_mouseX; }
		float GetY() const { return m_mouseY; }

		EventType GetType() const override { return AEngine::EventType::MouseMoved; }
		const char* GetName() const override { return "Mouse was moved"; }

	private:
		float m_mouseX, m_mouseY;
	};

	class MouseButtonPressedEvent : public Event
	{
	public:
		MouseButtonPressedEvent(const AEMouse button)
			: m_mouseButton(button) {}

		AEMouse GetButton() { return m_mouseButton; }

		EventType GetType() const override { return AEngine::EventType::MouseButtonPressed; }
		const char* GetName() const override { return "Mouse was pressed"; }

	private:
		AEMouse m_mouseButton;
	};

	class MouseButtonReleasedEvent : public Event
	{
	public:
		MouseButtonReleasedEvent(const AEMouse button)
			: m_mouseButton(button) {}

		AEMouse GetButton() { return m_mouseButton; }

		EventType GetType() const override { return AEngine::EventType::MouseButtonReleased; }
		const char* GetName() const override { return "Mouse was released"; }

	private:
		AEMouse m_mouseButton;
	};

	class MouseScrolledEvent : public Event
	{
	public:
		MouseScrolledEvent(const float x, const float y)
			: m_scrollX(x), m_scrollY(y) {}

		int GetScrollX() { return m_scrollX; }
		int GetScrollY() { return m_scrollY; }

		EventType GetType() const override { return AEngine::EventType::MouseScrolled; }
		const char* GetName() const override { return "Mouse was scrolled"; }

	private:
		float m_scrollX, m_scrollY;
	};
}