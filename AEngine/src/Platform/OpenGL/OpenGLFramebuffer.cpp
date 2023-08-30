#include "OpenGLFramebuffer.h"
#include "AEngine/Core/Logger.h"

namespace
{
	static constexpr GLenum g_glFramebufferMode[] = {
		GL_FRAMEBUFFER, GL_READ_FRAMEBUFFER, GL_DRAW_FRAMEBUFFER
	};
}

namespace AEngine
{
	OpenGLFramebuffer::OpenGLFramebuffer(Math::uvec2 windowSize)
	: m_width(windowSize.x), m_height(windowSize.y)
	{
		glGenFramebuffers(1, &m_framebuffer);
		GenerateTextures();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		if(!m_colorBuffers.empty())
			glDeleteTextures(m_colorBuffers.size(), m_colorBuffers.data());
	}

	void OpenGLFramebuffer::OnWindowResize(Math::uvec2 windowSize)
	{
		m_width = windowSize.x;
		m_height = windowSize.y;
		GenerateTextures();
	}

	void OpenGLFramebuffer::SetActiveDrawBuffers(const std::vector<unsigned int>& buffers)
	{
		m_activeBuffers.clear();
		for(auto index : buffers)
		{
			if(index < m_activeBuffers.size())
				m_activeBuffers.push_back(GL_COLOR_ATTACHMENT0 + index);
			else
				AE_LOG_FATAL("OpenGLFramebuffer::SetActuveDrawBuffers --> Index out of bounds");
		}
	}

	void OpenGLFramebuffer::GenerateTextures()
	{
		glBindTexture(GL_TEXTURE_2D, m_depthBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
		glBindTexture(GL_TEXTURE_2D, 0);

		glBindTexture(GL_TEXTURE_2D, m_stencilBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_STENCIL_INDEX, m_width, m_height, 0, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, nullptr);
		glBindTexture(GL_TEXTURE_2D, 0);

		glBindTexture(GL_TEXTURE_2D, m_depthStencilBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_width, m_height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
		glBindTexture(GL_TEXTURE_2D, 0);

		for(unsigned int texId : m_colorBuffers)
		{
			glBindTexture(GL_TEXTURE_2D, texId);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

	void OpenGLFramebuffer::Bind(FramebufferMode mode)
	{
		glBindFramebuffer(g_glFramebufferMode[static_cast<int>(mode)], m_framebuffer);
		if (!m_activeBuffers.empty())
			glDrawBuffers(m_activeBuffers.size(), m_activeBuffers.data());
	}

	void OpenGLFramebuffer::Unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Attach(FramebufferAttachment type, unsigned int index)
	{
		Bind();
		switch(type)
		{
			case FramebufferAttachment::Color:
				if(index > m_colorBuffers.size())
					AE_LOG_FATAL("OpenGLFramebuffer::Attach -> Index out of bounds (should be + 1 out of bounds for new textures)");

					// Need to create a new texture
				if(index == m_colorBuffers.size())
				{
					GLenum newTexture;
					glGenTextures(1, &newTexture);
					m_colorBuffers.push_back(newTexture);
				}
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, m_colorBuffers[index], 0);
				break;
			case FramebufferAttachment::Depth:
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthBuffer, 0);
				break;
			case FramebufferAttachment::DepthStencil:
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthStencilBuffer, 0);
				break;
			case FramebufferAttachment::Stencil:
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_stencilBuffer, 0);
				break;
			default:
				AE_LOG_FATAL("OpenGLFrameBuffer::Attach -> Error with type");
				break;
		}

		Unbind();
	}

	void OpenGLFramebuffer::Detach(FramebufferAttachment type, unsigned int index)
	{
		Bind();

		switch(type)
		{
			case FramebufferAttachment::Color:
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, 0, 0);
				break;
			case FramebufferAttachment::Depth:
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0);
				break;
			case FramebufferAttachment::DepthStencil:
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, 0, 0);
				break;
			case FramebufferAttachment::Stencil:
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, 0, 0);
				break;
			default:
				AE_LOG_FATAL("OpenGLFrameBuffer::Detach -> Error with type");
				break;
		}

		Unbind();
	}
}