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
	static constexpr GLenum g_glTextureWrapMode[] = {
		GL_REPEAT, GL_CLAMP_TO_EDGE, GL_CLAMP_TO_BORDER, GL_MIRRORED_REPEAT
	};

	static constexpr GLenum g_glTexureMinificationFilter[] = {
		GL_NEAREST, GL_LINEAR, GL_NEAREST_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_NEAREST,
		GL_NEAREST_MIPMAP_LINEAR, GL_LINEAR_MIPMAP_LINEAR
	};

	OpenGLTexture::OpenGLTexture(const std::string& ident, const std::string& fname)
		: Texture(ident, fname), m_id(0), m_width(0), m_height(0), m_nrChannels(0)
	{
		AE_LOG_DEBUG("OpenGLTexture::Constructor");
		Generate(fname);
	}

	OpenGLTexture::~OpenGLTexture()
	{
		AE_LOG_DEBUG("OpenGLTexture::Destructor");
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

		// Our default settings for all textures
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
			AE_LOG_FATAL("OpenGLTexture::Generate::Failed -> {}", fname);
		}

		// clean-up
		stbi_image_free(data);
		Unbind();
	}

	void OpenGLTexture::SetWrapS(TextureWrapMode mode)
	{
		Bind();
		GLenum gl_mode = g_glTextureWrapMode[static_cast<int>(mode)];
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, gl_mode);
		Unbind();
	}

	void OpenGLTexture::SetWrapT(TextureWrapMode mode)
	{
		Bind();
		GLenum gl_mode = g_glTextureWrapMode[static_cast<int>(mode)];
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, gl_mode);
		Unbind();
	}

	void OpenGLTexture::SetMinFilter(TextureMinificationFilter filter)
	{
		Bind();
		GLenum gl_filter = g_glTexureMinificationFilter[static_cast<int>(filter)];
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, gl_filter);
		Unbind();
	}

	void OpenGLTexture::SetMagnificationFilter(TextureMinificationFilter filter)
	{
		Bind();
		GLenum gl_filter = g_glTexureMinificationFilter[static_cast<int>(filter)];
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, gl_filter);
		Unbind();
	}

	void OpenGLTexture::SetTextureBaseLevel(int baseLevel)
	{
		Bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, baseLevel);
		Unbind();
	}

	void OpenGLTexture::SetTextureMaxLevel(int maxLevel) 
	{
		Bind();
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, maxLevel);
		Unbind();
	}

	void OpenGLTexture::SetTextureLODBias(float lodBias) 
	{
		Bind();
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, lodBias);
		Unbind();
	}

	void OpenGLTexture::SetTextureBorderColor(Math::vec4 borderColor) 
	{
		Bind();
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(borderColor));
		Unbind();
	}
}
