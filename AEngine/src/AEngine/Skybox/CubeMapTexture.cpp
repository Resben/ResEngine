/**
 * \file
 * \author Geoff Candy
 * \author Christien Alden (34119981)
*/
#include "CubeMapTexture.h"
#include "AEngine/Core/Logger.h"
#include <glad/glad.h>
#include <stb/stb_image.h>

namespace AEngine
{
	CubeMapTexture::CubeMapTexture(const std::vector<std::string>& texturePaths)
		: m_texture{ 0 }
	{
		glGenTextures(1, &m_texture);
		Bind();

		stbi_set_flip_vertically_on_load(false);

		// load textures
		int width, height, nrChannels;
		for (unsigned int i = 0; i < texturePaths.size(); i++)
		{
			unsigned char* data = stbi_load(texturePaths[i].c_str(), &width, &height, &nrChannels, 0);
			if (data)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
				stbi_image_free(data);
			}
			else
			{
				AE_LOG_ERROR("Cubemap texture failed to load at path: {}", texturePaths[i]);
				stbi_image_free(data);
			}
		}

		// set properties for texture
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		Unbind();
	}

	CubeMapTexture::~CubeMapTexture()
	{
		glDeleteTextures(1, &m_texture);
	}

	void CubeMapTexture::Bind(unsigned int unit) const
	{
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_texture);
	}

	void CubeMapTexture::Unbind() const
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}
}
