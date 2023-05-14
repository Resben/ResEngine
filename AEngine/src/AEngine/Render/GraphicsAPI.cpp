#include "GraphicsAPI.h"
#include "Platform/OpenGL/OpenGLGraphicsAPI.h"
#include "AEngine/Core/Logger.h"

namespace AEngine
{
	UniquePtr<GraphicsAPI> GraphicsAPI::Create(GraphicsType graphics)
	{
		switch (graphics)
		{
		case GraphicsType::OPENGL:
			return MakeUnique<OpenGLGraphicsAPI>();
		default:
			AE_LOG_FATAL("GraphicsAPI::Create::Error::GraphicsType -> Does not exist");
		}
	}
}
