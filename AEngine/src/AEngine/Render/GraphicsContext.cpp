#include <cstdlib>
#include "GraphicsContext.h"
#include "AEngine/Core/Window.h"
#include "AEngine/Core/Application.h"
#include "Platform/OpenGL/OpenGLGraphicsContext.h"

namespace AEngine
{
	std::unique_ptr<GraphicsContext> GraphicsContext::Create(void *window, WindowAPI api)
	{
		switch (api)
		{
		case WindowAPI::GLFW:
			return std::make_unique<OpenGLGraphicsContext>(window);
		}

		assert(false);
	}
}
