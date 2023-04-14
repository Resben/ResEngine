/**
 * @file
 * @author Ben Hawkins
 * @author Christien Alden (34119981)
 * @date 12/09/2022
**/
#include "../Core/Logger.h"
#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <filesystem>
#include "TextureManager.h"
namespace fs = std::filesystem;

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

	void TextureManager::LoadTextureDirectory(const std::string& dir)
	{
		// LoadTexture looks for '/' to determine texture name
		for (const auto& entry : fs::directory_iterator(dir + '/'))
		{
			if (is_regular_file(entry))
			{
				LoadTexture(fs::path(entry).string());
			}
		}
	}

	std::shared_ptr<Texture> TextureManager::LoadTexture(const std::string& filename)
	{
		stbi_set_flip_vertically_on_load(true);

		unsigned char* imgData;
		int width = 0;
		int height = 0;
		int channels = 0;
		imgData = stbi_load(filename.c_str(), &width, &height, &channels, 0);

		if (!imgData)
		{
			AE_LOG_ERROR("TextureManager::Load::Failed -> {}", filename);
			exit(1);
		}

		size_t index = filename.find_last_of('/');
		std::string texname = filename.substr(index + 1);

		// generate opengl texture
		m_textures.insert(std::make_pair(
			texname, std::make_shared<Texture>(imgData, width, height))
		);

		stbi_image_free(imgData);

		AE_LOG_TRACE("TextureManager::Load::Success -> {}", texname);
		return m_textures[texname];
	}

	Texture* TextureManager::GetTexture(const std::string& texName)
	{
		std::map<std::string, std::shared_ptr<Texture>>::iterator it;
		it = m_textures.find(texName);
		if (it != m_textures.end())
			return it->second.get();
		else
			return nullptr;
	}
}
