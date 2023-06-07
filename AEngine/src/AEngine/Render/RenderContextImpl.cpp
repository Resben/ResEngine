#include "RenderContextImpl.h"
#include "AEngine/Core/Logger.h"
#include "Platform/OpenGL/OpenGLRenderContext.h"

namespace AEngine
{
	UniquePtr<RenderContextImpl> RenderContextImpl::Create(const Window* window, WindowAPI api)
	{
		switch (api)
		{
		case WindowAPI::GLFW:
			return MakeUnique<OpenGLRenderContext>(window);
		default:
			AE_LOG_FATAL("RenderContext::Create::Error::WindowAPI -> Does not exist");
		}
	}
}
