#include "GraphicsAPI.h"
#include "Platform/OpenGL/OpenGLGraphicsAPI.h"
#include "AEngine/Core/Logger.h"

namespace AEngine
{
	UniquePtr<GraphicsAPI> GraphicsAPI::Create(GraphicsLibrary graphics)
	{
		switch (graphics)
		{
		case GraphicsLibrary::OpenGL:
			return MakeUnique<OpenGLGraphicsAPI>();
		default:
			AE_LOG_FATAL("GraphicsAPI::Create::Error::GraphicsLibrary -> Does not exist");
		}
	}
}
