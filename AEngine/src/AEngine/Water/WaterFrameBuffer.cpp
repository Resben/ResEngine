#include "WaterFrameBuffer.h"

namespace AEngine
{
	WaterFrameBuffer::WaterFrameBuffer()
	{
		reflectionFrameBuffer = CreateFrameBuffer();
		reflectionTexture = CreateTextureAttachment(REFLECTION_WIDTH, REFLECTION_HEIGHT);
		reflectionDepthBuffer = CreateDepthBufferAttachment(REFLECTION_WIDTH, REFLECTION_HEIGHT);
		unbindFrameBuffer();

		refractionFrameBuffer = CreateFrameBuffer();
		refractionTexture = CreateTextureAttachment(REFRACTION_WIDTH, REFRACTION_HEIGHT);
		refractionDepthTexture = CreateDepthTextureAttachment(REFRACTION_WIDTH, REFRACTION_HEIGHT);
		unbindFrameBuffer();
	}

	WaterFrameBuffer::~WaterFrameBuffer()
	{
		//glDeleteFramebuffers(1, &reflectionFrameBuffer);
		//glDeleteFramebuffers(1, &refractionFrameBuffer);
		//glDeleteRenderbuffers(1, &reflectionDepthBuffer);
		//glDeleteTextures(1, &refractionDepthTexture);

		delete reflectionTexture;
		delete refractionTexture;
	}

	void WaterFrameBuffer::clear()
	{
		//glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	}

	void WaterFrameBuffer::bindReflectionFrameBuffer()
	{
		//BindFrameBuffer(reflectionFrameBuffer, REFLECTION_WIDTH, REFLECTION_HEIGHT);
	}
	
	void WaterFrameBuffer::bindRefractionFrameBuffer()
	{
		//BindFrameBuffer(refractionFrameBuffer, REFRACTION_WIDTH, REFRACTION_HEIGHT);
	}

	void WaterFrameBuffer::unbindFrameBuffer()
	{
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		//glViewport(0, 0, windowWidth, windowHeight);
	}

	Texture *WaterFrameBuffer::getReflectionTexture()
	{
		return reflectionTexture;
	}
	
	Texture *WaterFrameBuffer::getRefractionTexture()
	{
		return refractionTexture;
	}
	
	unsigned int WaterFrameBuffer::getRefractionDepthTexture()
	{
		return refractionDepthTexture;
	}
	
	//PRIVATE METHODS
	unsigned int WaterFrameBuffer::CreateFrameBuffer()
	{
		unsigned int framebuffer = 0;
		//glGenFramebuffers(1, &framebuffer);
		//glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		//glDrawBuffer(GL_COLOR_ATTACHMENT);

		return framebuffer;
	}
	
	
	Texture *WaterFrameBuffer::CreateTextureAttachment(int width, int height)
	{
		Texture*  texture;
		//texture->width = width;
		//texture->heigth = height;
		//texture->hasTransparency = false;
		
		//glGenTexture(1, &(texture->GetWidth));
		//glBindTexture(GL_TEXTURE_2D, texture->GetIdent());
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture->GetIdent(), 0);
		//return texture;
	}
}
