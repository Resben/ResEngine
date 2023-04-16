#include <cstdlib>
#include "GraphicsContext.h"
#include "AEngine/Core/Window.h"
#include "AEngine/Core/Application.h"
#include "Platform/OpenGL/OpenGLGraphicsContext.h"

namespace AEngine
{
	std::unique_ptr<GraphicsContext> GraphicsContext::Create(void *window)
	{
		switch (Application::instance().GetWindow().GetAPI())
		{
		case WindowAPI::GLFW:
			return std::unique_ptr<OpenGLGraphicsContext>(window);
		}

		assert(false);
	}
}
