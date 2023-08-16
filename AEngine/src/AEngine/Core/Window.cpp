/**
 * \file
 * \author Christien Alden (34119981)
*/
#include "Window.h"
#include "AEngine/Input/Input.h"
#include "Logger.h"
#include "AEngine/Events/ApplicationEvent.h"

#ifdef AE_PLATFORM_WINDOWS
	#include "Platform/Windows/WindowsWindow.h"
#endif

namespace AEngine
{
	Window::Window(Properties properties)
		: m_properties{ properties }, m_eventHandler{ }
	{
		AE_LOG_INFO("Window::Constructor");
		RegisterEventHandler<WindowResized>(1, [&, this](WindowResized& event) -> bool {
			m_properties.width = event.GetWidth();
			m_properties.height = event.GetHeight();
			return false;
		});
	}

	UniquePtr<Window> Window::Create(const Properties& properties)
	{
		AE_LOG_INFO("Window::Create");

#ifdef AE_PLATFORM_WINDOWS
		return MakeUnique<WindowsWindow>(properties);
#else
		AE_LOG_FATAL("Platform not implemented");
#endif
	}

	Math::uvec2 Window::GetSize() const
	{
		return { m_properties.width, m_properties.height };
	}

	void Window::MakeCurrent() const
	{
		RenderContext::MakeCurrent(this);
	}

	void Window::SwapBuffers() const
	{
		RenderContext::SwapBuffers(this);
	}

	void Window::ShowCursor(bool toggle) const
	{
		RenderContext::ShowCursor(this, toggle);
	}

	bool Window::IsShowingCursor() const
	{
		return RenderContext::IsShowingCursor(this);
	}

	const std::string &Window::GetTitle() const
	{
		return m_properties.title;
	}

	bool Window::IsVisible() const
	{
		return m_properties.visible;
	}
}
