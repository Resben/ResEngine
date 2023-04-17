#include "Texture.h"
#include "Platform/OpenGL/OpenGLTexture.h"	
#include "AEngine/Core/Application.h"
#include "AEngine/Core/Logger.h"

namespace AEngine
{
	std::shared_ptr<Texture> AEngine::Texture::Create(const std::string& fname)
	{
		switch (Application::Instance().Graphics().GetType())
		{
		case GraphicsType::OPENGL:
			return std::make_shared<OpenGLTexture>(fname);
		default:
			AE_LOG_FATAL("Texture::Create::GraphicsType::Error -> None selected");
		}
	}
}
