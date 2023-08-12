#include "Animation.h"
#include "AEngine/Core/Application.h"
#include "AEngine/Core/Logger.h"
#include "AEngine/Render/RenderCommand.h"
#include "Platform/Assimp/AssimpAnimation.h"

namespace AEngine
{
	Animation::Animation(const std::string& ident, const std::string& parent)
		: Asset(ident, parent)
	{

	}

	SharedPtr<Animation> AEngine::Animation::Create(const std::string& ident, const std::string& parent, const SharedPtr<Animation> animation)
	{
		switch (RenderCommand::GetLibrary())
		{
		case RenderLibrary::OpenGL:
			return MakeShared<AssimpAnimation>(ident, parent, animation);
		default:
			AE_LOG_FATAL("Texture::Create::RenderLibrary::Error -> None selected");
		}
	}
}