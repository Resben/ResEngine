#pragma once
#include "AEngine/Render/Framebuffer.h"
#include <glad/glad.h>
#include <vector>

namespace AEngine
{
	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(unsigned int width, unsigned int height);

		virtual void OnWindowResize(unsigned int width, unsigned int height) override;
		virtual void Bind(FramebufferMode mode = FramebufferMode::ReadWrite) override;
		virtual void Unbind() const override;
		virtual void Attach(FramebufferAttachment type, unsigned int index = 0) override;
		virtual void Detach(FramebufferAttachment type, unsigned int index = 0) override;

	private:
		void GenerateTextures();

		unsigned int m_framebuffer;
		std::vector<unsigned int> m_colorBuffers;
		unsigned int m_depthBuffer, m_stencilBuffer, m_depthStencilBuffer;
		unsigned int m_width, m_height;
	};
}