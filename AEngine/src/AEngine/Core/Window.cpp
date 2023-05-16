/**
 * @file
 * @author Christien Alden (34119981)
 * @brief Window interface implementation
**/
#include "Window.h"
#include "AEngine/Input/Input.h"
#include "Logger.h"

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

	UniquePtr<Window> Window::Create(const WindowProps& props)
	{
		AE_LOG_INFO("Window::Create");

#ifdef AE_PLATFORM_WINDOWS
		return std::make_unique<WindowsWindow>(props);
#else
		AE_LOG_FATAL("Platform not implemented");
#endif
	}
}
