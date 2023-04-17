#include "GraphicsContext.h"
#include "AEngine/Core/Window.h"
#include "Platform/OpenGL/OpenGLGraphicsContext.h"
#include "AEngine/Core/Logger.h"

namespace AEngine
{
	std::unique_ptr<GraphicsContext> GraphicsContext::Create(void *window, WindowAPI api)
	{
		switch (api)
		{
		case WindowAPI::GLFW:
			return std::make_unique<OpenGLGraphicsContext>(window);
		default:
			AE_LOG_FATAL("Texture::Create::Graphics type does not exist");
		}

		assert(false);
	}
}
