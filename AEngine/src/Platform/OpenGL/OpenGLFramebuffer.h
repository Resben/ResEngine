#pragma once
#include "AEngine/Render/Framebuffer.h"
#include <glad/glad.h>

namespace AEngine
{
		/**
		 * \class OpenGLFramebuffer
		 * \brief OpenGL implementation of Framebuffer
		 * \author Ben Hawkins (34112619) 
		**/
	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(Math::uvec2 size);
		~OpenGLFramebuffer() override;

			/**
			 * \copydoc FrameBuffer::TransferDepthBuffer
			**/
		virtual void TransferDepthBuffer(unsigned int dest) override;
			/**
			 * \copydoc FrameBuffer::SetActiveDrawBuffers
			**/
		virtual void SetActiveDrawBuffers(const std::vector<unsigned int>& buffers) override;
			/**
			 * \copydoc FrameBuffer::ResizeBuffers
			**/
		virtual void ResizeBuffers(Math::uvec2 size) override;
			/**
			 * \copydoc FrameBuffer::BindBuffers
			**/
		virtual void BindBuffers(const std::vector<unsigned int>& buffers) override;
			/**
			 * \copydoc FrameBuffer::UnbindBuffers
			**/
		virtual void UnbindBuffers() override;
			/**
			 * \copydoc FrameBuffer::Bind
			**/
		virtual void Bind(FramebufferMode mode = FramebufferMode::ReadWrite) override;
			/**
			 * \copydoc FrameBuffer::Unbind
			**/
		virtual void Unbind() override;
			/**
			 * \copydoc FrameBuffer::Attach
			**/
		virtual void Attach(FramebufferAttachment type, unsigned int index = 0) override;
			/**
			 * \copydoc FrameBuffer::Detach
			**/
		virtual void Detach(FramebufferAttachment type, unsigned int index = 0) override;

	private:
			/**
			 * \brief Debugging function to check if a buffer was correctly attached
			 * \retval void 
			**/
		void CheckFramebufferStatus();

		unsigned int m_framebuffer;
		std::vector<unsigned int> m_colorBuffers;
		std::vector<GLenum> m_activeBuffers;
		unsigned int m_depthBuffer, m_stencilBuffer, m_depthStencilBuffer;
		unsigned int m_width, m_height;
	};
}