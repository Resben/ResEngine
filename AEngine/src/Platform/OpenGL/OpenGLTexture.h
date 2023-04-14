#pragma once
#include <string>
#include <glad/glad.h>
#include "AEngine/Render/Texture.h"

namespace AEngine
{
		/**
		 * @class Texture
		 * @brief Abstract texture object
		 * @author Christien Alden (34119981)
		**/
	class OpenGLTexture : public Texture
	{
	public:
		OpenGLTexture(const std::string &fname);
		virtual ~OpenGLTexture();

			/**
			 * @brief Binds the texture to the rendering API
			 * @return void
			**/
		void Bind(unsigned int unit = 0) const;
			/**
			 * @brief Unbinds the texture from the rendering API
			 * @return void
			**/
		void Unbind() const;

		int GetWidth() const;
		int GetHeight() const;

	private:
		GLuint m_id;
		int m_width;
		int m_height;
		int m_nrChannels;

		void Generate(const std::string& fname);
	};
}
