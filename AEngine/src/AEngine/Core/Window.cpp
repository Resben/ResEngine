/**
 * @file
 * @author Christien Alden (34119981)
 * @brief Window interface implementation
**/
#include "Logger.h"
#include "Window.h"

#ifdef AE_PLATFORM_WINDOWS
	#include "Platform/Windows/WindowsWindow.h"
#endif

namespace AEngine
{
	Window::Window(WindowProps props)
		: m_props(props)
	{
		AE_LOG_INFO("Window::Constructor");
	}

	std::unique_ptr<Window> Window::Create(const WindowProps& props)
	{
		AE_LOG_INFO("Window::Create");

#ifdef AE_PLATFORM_WINDOWS
		return std::make_unique<WindowsWindow>(props);
#else
		AE_LOG_FATAL("Platform not implemented");
#endif
	}
}
