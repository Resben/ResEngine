/**
 *	@file		
 *	@brief		Texture manager to load a directory of textures
 *  @author		Ben Hawkins
 *  @author		Christien Alden (34119981)
 *  @date		12/09/2022
**/
#pragma once
#ifndef IMAGEPROCESSOR_H_INCLUDED
#define IMAGEPROCESSOR_H_INCLUDED
#include <string>
#include <map>
#include <memory>
#include "../Render/Texture.h"

namespace AEngine
{
		/**
		 * @class TextureManager
		 * @brief Holds shared pointers to loaded Textures
		 * @author Christien Alden (34119981)
		 * @author Ben Hawking
		**/
	class TextureManager
	{
	public:
			/**
			 * @retval TextureManager* Instance of TextureManager
			**/
		static TextureManager* Instance();

			/**
			 * @brief Clears loaded Textures
			 * @retval void
			**/
		void Clear();

			/**
			 *	@brief		Loads a texture from an image file
			 *	@param[in]	filename of image to load
			 *	@retval		std::shared_ptr<Texture> to texture just loaded
			**/
		std::shared_ptr<Texture> LoadTexture(const std::string& path);

			/**
			 * @brief Searches loaded Textures for a given texture
			 * @param[in] texName to search for
			 * @retval shared_ptr of texture, nullptr is not found
			**/
		std::shared_ptr<Texture> GetTexture(const std::string& texName);

		TextureManager(const TextureManager&) = delete;
		void operator=(const TextureManager&) = delete;

	private:
		static TextureManager* s_instance;
		TextureManager() = default;

			/// Map of texture names to Texture objects
		std::map<std::string, std::shared_ptr<Texture>> m_textures;
	};
}

#endif
