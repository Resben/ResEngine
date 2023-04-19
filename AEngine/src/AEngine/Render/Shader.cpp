#include "Shader.h"
#include "AEngine/Core/Application.h"
#include "Platform/OpenGL/OpenGLShader.h"
#include "AEngine/Core/Logger.h"

namespace AEngine
{
	Shader::Shader(const std::string& ident, const std::string& path)
		: Asset(ident, path)
	{

	}

	std::shared_ptr<Shader> AEngine::Shader::Create(const std::string& ident, const std::string& fname)
	{
		switch (Application::Instance().Graphics().GetType())
		{
		case GraphicsType::OPENGL:
			return std::make_shared<OpenGLShader>(ident, fname);
		default:
			AE_LOG_FATAL("Shader::Create::GraphicsType::Error -> None selected");
		}
	}
}

