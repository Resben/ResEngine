/**
 * \file
 * \author Christien Alden (34119981)
*/
#include "Window.h"
#include "AEngine/Input/Input.h"
#include "Logger.h"

#ifdef AE_PLATFORM_WINDOWS
	#include "Platform/Windows/WindowsWindow.h"
#endif

namespace AEngine
{
	Window::Window(Properties properties)
		: m_properties{ properties }
	{
		AE_LOG_INFO("Window::Constructor");
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
}
