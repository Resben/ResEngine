#include "GraphicsContextImpl.h"
#include "AEngine/Core/Logger.h"
#include "Platform/OpenGL/OpenGLGraphicsContext.h"

namespace AEngine
{
	UniquePtr<GraphicsContextImpl> GraphicsContextImpl::Create(const Window* window, WindowAPI api)
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
