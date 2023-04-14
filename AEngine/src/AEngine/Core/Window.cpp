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
	std::unique_ptr<Window> Window::Create(const WindowProps& props)
	{
#ifdef AE_PLATFORM_WINDOWS
		return std::make_unique<WindowsWindow>(props);
#else
		AE_LOG_CRITICAL("Platform not implemented");
#endif
	}
}
