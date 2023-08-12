#include "Model.h"
#include "AEngine/Core/Application.h"
#include "AEngine/Core/Logger.h"
#include "AEngine/Render/RenderCommand.h"
#include "Platform/Assimp/AssimpModel.h"

namespace AEngine
{
	Model::Model(const std::string& ident, const std::string& path)
		: Asset(ident, path)
	{

	}

	SharedPtr<Model> AEngine::Model::Create(const std::string& ident, const std::string& fname)
	{
		switch (RenderCommand::GetLibrary())
		{
		case RenderLibrary::OpenGL:
			return MakeShared<AssimpModel>(ident, fname);
		default:
			AE_LOG_FATAL("Texture::Create::RenderLibrary::Error -> None selected");
		}
	}
}