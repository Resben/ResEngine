#include "GraphicsAPI.h"
#include "Platform/OpenGL/OpenGLGraphicsAPI.h"
#include "AEngine/Core/Logger.h"

namespace AEngine
{
	std::unique_ptr<GraphicsAPI> GraphicsAPI::Create(GraphicsType graphics)
	{
		switch (graphics)
		{
		case GraphicsType::OPENGL:
			return std::make_unique<OpenGLGraphicsAPI>();
		default:
			AE_LOG_FATAL("Texture::Create::Graphics type does not exist");
		}
	}
}
