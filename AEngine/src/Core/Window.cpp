/**
 * @file
 * @author Christien Alden (34119981)
 * @brief Window interface implementation
**/
#include "Window.h"
#include "GLWindow.h"

namespace AEngine
{
	std::unique_ptr<Window> Window::Create(const WindowProps& props)
	{
		return std::make_unique<GLWindow>(props);
	}
}
