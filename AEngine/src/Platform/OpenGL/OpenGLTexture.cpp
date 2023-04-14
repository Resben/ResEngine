/**
 * @file
 * @author Christien Alden (34119981)
 * @brief Abstract Texture object
**/
#include "AEngine/Core/Logger.h"
#include "OpenGLTexture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>


namespace AEngine
{
	OpenGLTexture::OpenGLTexture(const std::string &fname)
		: m_id(0), m_width(0), m_height(0), m_nrChannels(0)
	{
		Generate(fname);
	}

	OpenGLTexture::~OpenGLTexture()
	{
		 glDeleteTextures(1, &m_id);
		 m_id = 0;
	}

	int OpenGLTexture::GetWidth() const
	{
		return m_width;
	}

	int OpenGLTexture::GetHeight() const
	{
		return m_height;
	}

	// unsure whether we keep this or can just rely on the shader uniforms...
	void OpenGLTexture::Bind(unsigned int unit) const
	{
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_2D, m_id);
	}

	void OpenGLTexture::Unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void OpenGLTexture::Generate(const std::string& fname)
	{
		// generate and bind
		glGenTextures(1, &m_id);
		Bind();

		// set properties for texture
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// generate texture
		unsigned char* data = stbi_load(fname.c_str(), &m_width, &m_height, &m_nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			AE_LOG_ERROR("OpenGLTexture::Load_Failed -> {}", fname);
			exit(1);
		}

		// clean-up
		stbi_image_free(data);
		Unbind();
	}
}
