#include "GraphicsContext.h"
#include "AEngine/Core/Logger.h"
#include "AEngine/Core/Window.h"
#include "Platform/OpenGL/OpenGLGraphicsContext.h"

namespace AEngine
{
	UniquePtr<GraphicsContext> GraphicsContext::Create(void *window, WindowAPI api)
	{
		switch (api)
		{
		case WindowAPI::GLFW:
			return MakeUnique<OpenGLGraphicsContext>(window);
		default:
			AE_LOG_FATAL("GraphicsContext::Create::Error::WindowAPI -> Does not exist");
		}
	}
}
