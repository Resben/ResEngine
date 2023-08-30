#pragma once
#include "AEngine/Render/Framebuffer.h"
#include <glad/glad.h>

namespace AEngine
{
	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer();

		virtual void Bind(FramebufferMode mode = FramebufferMode::ReadWrite) const override;
		virtual void Unbind() const override;
		virtual void Attach(FramebufferAttachment type, unsigned int index = 0) override;
		virtual void Detach(FramebufferAttachment type, unsigned int index = 0) override;

	private:
		unsigned int m_framebuffer;
	};
}