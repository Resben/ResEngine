#pragma once
#include "GraphicsAPI.h"

#ifdef AE_RENDER_OPENGL
	#include "Platform/OpenGL/OpenGLGraphicsAPI.h"
#endif

namespace AEngine
{
	std::unique_ptr<GraphicsAPI> GraphicsAPI::Create()
	{
#ifdef AE_RENDER_OPENGL
		return std::make_unique<OpenGLGraphicsAPI>();
#else
	#error "Graphics API not supported!"
#endif

	}
}
