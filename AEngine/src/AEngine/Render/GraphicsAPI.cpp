#include "GraphicsAPI.h"
#include "Platform/OpenGL/OpenGLGraphicsAPI.h"
#include "AEngine/Core/Logger.h"

namespace AEngine
{
	UniquePtr<GraphicsAPI> GraphicsAPI::Create(RenderLibrary graphics)
	{
		switch (graphics)
		{
		case RenderLibrary::OpenGL:
			return MakeUnique<OpenGLGraphicsAPI>();
		default:
			AE_LOG_FATAL("GraphicsAPI::Create::Error::RenderLibrary -> Does not exist");
		}
	}
}
