#pragma once
#include "AEngine/Core/KeyCodes.h"
#include "Event.h"

namespace AEngine
{
	class MouseMoved : public Event
	{
	public:
		MouseMoved(const float x, const float y)
		: m_mouseX(x), m_mouseY(y) {}

		float GetX() const { return m_mouseX; }
		float GetY() const { return m_mouseY; }
		EventType GetType() const override { return EventType::MouseMoved; }
		const char* GetName() const override { return "MouseMoved"; }
		static EventType GetStaticType() { return EventType::MouseMoved; }

	private:
		float m_mouseX, m_mouseY;
	};

	class MouseButtonPressed : public Event
	{
	public:
		MouseButtonPressed(const AEMouse button)
			: m_mouseButton(button) {}

		AEMouse GetButton() { return m_mouseButton; }
		EventType GetType() const override { return EventType::MouseButtonPressed; }
		const char* GetName() const override { return "MouseButtonPressed"; }
		static EventType GetStaticType() { return EventType::MouseButtonPressed; }

	private:
		AEMouse m_mouseButton;
	};

	class MouseButtonReleased : public Event
	{
	public:
		MouseButtonReleased(const AEMouse button)
			: m_mouseButton(button) {}

		AEMouse GetButton() { return m_mouseButton; }
		EventType GetType() const override { return EventType::MouseButtonReleased; }
		const char* GetName() const override { return "MouseButtonReleased"; }
		static EventType GetStaticType() { return EventType::MouseButtonReleased; }

	private:
		AEMouse m_mouseButton;
	};

	class MouseScrolled : public Event
	{
	public:
		MouseScrolled(const float x, const float y)
			: m_scrollX(x), m_scrollY(y) {}

		float GetScrollX() { return m_scrollX; }
		float GetScrollY() { return m_scrollY; }
		EventType GetType() const override { return EventType::MouseScrolled; }
		const char* GetName() const override { return "MouseScrolled"; }
		static EventType GetStaticType() { return EventType::MouseScrolled; }

	private:
		float m_scrollX, m_scrollY;
	};
}