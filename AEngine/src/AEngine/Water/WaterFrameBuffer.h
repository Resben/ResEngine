#pragma once

#include "AEngine/Render/Texture.h"

namespace AEngine
{
	class WaterFrameBuffer
	{
	public:
		WaterFrameBuffer();

		~WaterFrameBuffer();

		void clear();

		void bindReflectionFrameBuffer();

		void bindRefractionFrameBuffer();

		void unbindFrameBuffer();

		Texture* getReflectionTexture();

		Texture* getRefractionTexture();

		unsigned int getRefractionDepthTexture();

	private:

		//Reflection Resolution
		const int REFLECTION_WIDTH = 320;
		const int REFLECTION_HEIGHT = 180;

		//Refraction Resolution
		const int REFRACTION_WIDTH = 1280;
		const int REFRACTION_HEIGHT = 720;

		//Reflection FrameBuffer stuff
		unsigned int reflectionFrameBuffer;
		Texture* reflectionTexture;
		unsigned int reflectionDepthBuffer;

		//Refraction FrameBuffer stuff
		unsigned int refractionFrameBuffer;
		Texture* refractionTexture;
		unsigned int refractionDepthTexture;

		unsigned int CreateFrameBuffer();

		Texture* CreateTextureAttachment(int width, int height);

		unsigned int CreateDepthTextureAttachment(int width, int height);

		unsigned int CreateDepthBufferAttachment(int width, int height);

		void bindFrameBuffer(unsigned int framebuffer, int width, int height);


	};
}