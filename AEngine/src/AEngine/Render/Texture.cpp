#include "Texture.h"
#include "AEngine/Core/Application.h"
#include "AEngine/Core/Logger.h"
#include "AEngine/Render/RenderCommand.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace AEngine
{
	Texture::Texture(const std::string& ident, const std::string& path)
		: Asset(ident, path)
	{

	}

	SharedPtr<Texture> AEngine::Texture::Create(const std::string& ident, const std::string& fname)
	{
		switch (RenderCommand::GetLibrary())
		{
		case RenderLibrary::OpenGL:
			return MakeShared<OpenGLTexture>(ident, fname);
		default:
			AE_LOG_FATAL("Texture::Create::RenderLibrary::Error -> None selected");
		}
	}
}
