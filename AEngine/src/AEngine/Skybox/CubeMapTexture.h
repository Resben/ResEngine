/**
 * \file
 * \author Geoff Candy
 * \author Christien Alden (34119981)
*/
#pragma once
#include <string>
#include <vector>

namespace AEngine
{
		/**
		 * \class CubeMapTexture
		 * \brief Represents a cubemap texture
		*/
	class CubeMapTexture
	{
	public:
			/**
			 * \param[in] texturePaths Paths to the cubemap textures
			*/
		CubeMapTexture(const std::vector<std::string>& texturePaths);
		~CubeMapTexture();

			/**
			 * \brief Binds the texture to the rendering API
			 * \param[in] unit The texture unit to bind to, defaults to 0
			*/
		void Bind(unsigned int unit = 0) const;
			/**
			 * \brief Unbinds the texture from the rendering API
			*/
		void Unbind() const;

	private:
			/**
			 * \brief OpenGL texture handle
			*/
		unsigned int m_texture;
	};
}
