#include "Texture.h"
#include "Platform/OpenGL/OpenGLTexture.h"	
#include "AEngine/Core/Application.h"
#include "AEngine/Core/Logger.h"

namespace AEngine
{
	Texture::Texture(const std::string& ident, const std::string& path)
		: Asset(ident, path)
	{

	}

	std::shared_ptr<Texture> AEngine::Texture::Create(const std::string& ident, const std::string& fname)
	{
		switch (Application::Instance().Graphics().GetType())
		{
		case GraphicsType::OPENGL:
			return std::make_shared<OpenGLTexture>(ident, fname);
		default:
			AE_LOG_FATAL("Texture::Create::GraphicsType::Error -> None selected");
		}
	}
}
