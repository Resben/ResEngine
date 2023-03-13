/**
 * @file
 * @author Christien Alden (34119981)
 * @brief Abstract Texture object
**/
#include <glad/glad.h>
#include "Texture.h"

namespace AEngine
{
	Texture::Texture(unsigned char* imgData, int width, int height)
		: m_id(0)
	{
		Generate(imgData, width, height);
	}

	Texture::~Texture()
	{
		 glDeleteTextures(1, &m_id);
		 m_id = 0;
	}

	void Texture::Bind() const
	{
		glBindTexture(GL_TEXTURE_2D, m_id);
	}

	void Texture::Unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture::Generate(unsigned char* imgData, int width, int height)
	{
		// generate buffer
		glGenTextures(1, &m_id);
		Bind();

		// generate texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imgData);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);

		Unbind();
	}
}
