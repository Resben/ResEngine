#pragma once
#include "AEngine/Render/Framebuffer.h"
#include <glad/glad.h>

namespace AEngine
{
	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(Math::uvec2 size);
		~OpenGLFramebuffer() override;

		virtual void TransferDepthBuffer(unsigned int dest) override;
		virtual void SetActiveDrawBuffers(const std::vector<unsigned int>& buffers) override;
		virtual void ResizeBuffers(Math::uvec2 size) override;
		virtual void BindBuffers(const std::vector<unsigned int>& buffers) override;
		virtual void UnbindBuffers() override;
		virtual void Bind(FramebufferMode mode = FramebufferMode::ReadWrite) override;
		virtual void Unbind() override;
		virtual void Attach(FramebufferAttachment type, unsigned int index = 0) override;
		virtual void Detach(FramebufferAttachment type, unsigned int index = 0) override;

	private:
		void GenerateTextures();
		void ClearTextures();

		void CheckFramebufferStatus();
		void ReadBuffer();

		unsigned int m_framebuffer;
		std::vector<unsigned int> m_colorBuffers;
		std::vector<GLenum> m_activeBuffers;
		unsigned int m_depthBuffer, m_stencilBuffer, m_depthStencilBuffer;
		unsigned int m_width, m_height;
	};
}