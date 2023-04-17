#include "../Core/Logger.h"
#include "ShaderManager.h"

#ifdef AE_RENDER_OPENGL
	#include "Platform/OpenGL/OpenGLShader.h"
#endif

namespace AEngine
{
	ShaderManager* ShaderManager::s_instance = nullptr;

	ShaderManager* ShaderManager::Instance()
	{
		if (!s_instance)
		{
			s_instance = new ShaderManager;
		}

		return s_instance;
	}

	void ShaderManager::Clear()
	{
		AE_LOG_DEBUG("ShaderManager::Clear");
		m_shaders.clear();
	}

	std::shared_ptr<Shader> ShaderManager::LoadShader(
		const std::string& name,
		const std::string& fileName)
	{
		m_shaders.emplace(std::make_pair(
			name, Shader::Create(fileName))
		);

		AE_LOG_TRACE("ShaderManager::LoadShader::Success -> {}", name);
		return m_shaders[name];
	}

	std::shared_ptr<Shader> ShaderManager::GetShader(const std::string& shaderName)
	{
		std::map<std::string, std::shared_ptr<Shader>>::iterator it;
		it = m_shaders.find(shaderName);
		if (it != m_shaders.end())
			return it->second;
		else
			return nullptr;
	}
}
