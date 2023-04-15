#include "Platform/Windows/WindowsKeys.h"
#include "Event.h"

namespace AEngine
{
	class WindowResizeEvent : public Event
	{
	public:
		WindowResizeEvent(unsigned int width, unsigned int height)
			: m_width(width), m_height(height) {}

		unsigned int GetWidth() const { return m_width; }
		unsigned int GetHeight() const { return m_height;  }

		EventType GetType() const override { return  AEngine::EventType::WindowResized; }
		const char* GetName() const override { return "Window resizeed"; }

	private:
		unsigned int m_width, m_height;
	};

	class WindowClosedEvent : public Event
	{
	public:
		WindowClosedEvent() = default;

		EventType GetType() const override { return  AEngine::EventType::WindowClosed; }
		const char* GetName() const override { return "Window closed"; }
	};
}