/**
 * @file
 * @author Ben Hawkins
 * @author Christien Alden (34119981)
 * @date 12/09/2022
**/
#include "AEngine/Core/Logger.h"
#include <filesystem>
#include "TextureManager.h"
namespace fs = std::filesystem;

#ifdef AE_RENDER_OPENGL
	#include "Platform/OpenGL/OpenGLTexture.h"
#endif

namespace AEngine
{
	TextureManager* TextureManager::s_instance = nullptr;

	TextureManager* TextureManager::Instance()
	{
		if (!s_instance)
		{
			s_instance = new TextureManager;
		}

		return s_instance;
	}

	void TextureManager::Clear()
	{
		AE_LOG_DEBUG("TextureManager::Clear");
		m_textures.clear();
	}

	std::shared_ptr<Texture> TextureManager::LoadTexture(const std::string& filename)
	{
		size_t index = filename.find_last_of('/');
		const std::string texname = filename.substr(index + 1);

		// generate opengl texture
		m_textures.insert(std::make_pair(
			texname, Texture::Create(texname, filename))
		);

		AE_LOG_TRACE("TextureManager::LoadTexture::Success -> {}", texname);
		return m_textures[texname];
	}

	std::shared_ptr<Texture> TextureManager::GetTexture(const std::string& texName)
	{
		std::map<std::string, std::shared_ptr<Texture>>::iterator it;
		it = m_textures.find(texName);
		if (it != m_textures.end())
			return it->second;
		else
			return nullptr;
	}
}
