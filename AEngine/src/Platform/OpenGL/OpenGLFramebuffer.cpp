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

	// --------------------------- DEBUGGING ------------------------------------
	void OpenGLFramebuffer::CheckFramebufferStatus()
	{
		GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (status != GL_FRAMEBUFFER_COMPLETE)
		{
			std::string errorMsg;
			switch (status)
			{
			case GL_FRAMEBUFFER_UNDEFINED: errorMsg = "GL_FRAMEBUFFER_UNDEFINED"; break;
			case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: errorMsg = "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT"; break;
			case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: errorMsg = "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT"; break;
			case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER: errorMsg = "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER"; break;
			case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER: errorMsg = "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER"; break;
			case GL_FRAMEBUFFER_UNSUPPORTED: errorMsg = "GL_FRAMEBUFFER_UNSUPPORTED"; break;
			case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE: errorMsg = "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE"; break;

			default: errorMsg = "UNKNOWN_ERROR"; break;
			}
			AE_LOG_ERROR("Framebuffer is not complete: {}", errorMsg);
		}
		else
		{
			AE_LOG_DEBUG("Framebuffer is complete.");
		}
	}

	void OpenGLFramebuffer::ReadBuffer()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
		glReadBuffer(GL_COLOR_ATTACHMENT0);
		std::vector<unsigned char> pixelData(m_width * m_height * 4);
		glReadPixels(0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, pixelData.data());

		bool hasContent = false;
		for (int i = 0; i < m_width * m_height * 4; i++) {
			if (pixelData[i] == 73) {
				AE_LOG_DEBUG("Content {}: {}/{}", pixelData[i], i, m_height * m_width * 4);
				hasContent = true;
				break;
			}
		}
		if (hasContent) {
			AE_LOG_DEBUG("Framebuffer has content!");
		}
		else {
			AE_LOG_DEBUG("Framebuffer is empty!");
		}
	}

	// --------------------------- DEBUGGING ------------------------------------

	OpenGLFramebuffer::OpenGLFramebuffer(Math::uvec2 size)
	: m_width(size.x), m_height(size.y), m_depthBuffer(0), m_depthStencilBuffer(0), m_stencilBuffer(0)
	{
		glGenFramebuffers(1, &m_framebuffer);
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		ClearTextures();
	}

	void OpenGLFramebuffer::ClearTextures()
	{
		if (!m_colorBuffers.empty())
			glDeleteTextures(m_colorBuffers.size(), m_colorBuffers.data());

		if(m_depthBuffer != 0)
			glDeleteTextures(GL_TEXTURE_2D, &m_depthBuffer);

		if (m_stencilBuffer != 0)
			glDeleteTextures(GL_TEXTURE_2D, &m_stencilBuffer);

		if (m_depthStencilBuffer != 0)
			glDeleteTextures(GL_TEXTURE_2D, &m_depthStencilBuffer);
	}

	void OpenGLFramebuffer::TransferDepthBuffer(unsigned int dest)
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_framebuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dest);

		glBlitFramebuffer(
			0, 0, m_width, m_height,
			0, 0, m_width, m_height,
			GL_DEPTH_BUFFER_BIT,
			GL_NEAREST
		);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::ResizeBuffers(Math::uvec2 size)
	{
		m_width = size.x;
		m_height = size.y;
		GenerateTextures();
	}

	void OpenGLFramebuffer::SetActiveDrawBuffers(const std::vector<unsigned int>& buffers)
	{
		m_activeBuffers.clear();

		for(auto index : buffers)
		{
			if(index < m_colorBuffers.size())
				m_activeBuffers.push_back(GL_COLOR_ATTACHMENT0 + index);
			else
				AE_LOG_FATAL("OpenGLFramebuffer::SetActiveDrawBuffers --> Index out of bounds");
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

	void OpenGLFramebuffer::BindBuffers(const std::vector<unsigned int>& buffers)
	{
		Unbind();
		for(auto index : buffers)
		{
			if(index < m_colorBuffers.size())
			{
				glActiveTexture(GL_TEXTURE0 + index);
				glBindTexture(GL_TEXTURE_2D, m_colorBuffers[index]);
			}
			else
				AE_LOG_ERROR("OpenGLFramebuffer::BindBuffers --> Index out of bounds");
		}
	}

	void OpenGLFramebuffer::UnbindBuffers()
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	} 

	void OpenGLFramebuffer::Bind(FramebufferMode mode)
	{
		glBindFramebuffer(g_glFramebufferMode[static_cast<int>(mode)], m_framebuffer);
		if (!m_activeBuffers.empty())
		{
			glDrawBuffers(m_activeBuffers.size(), m_activeBuffers.data());
		}
	}

	void OpenGLFramebuffer::Unbind()
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
					glBindTexture(GL_TEXTURE_2D, newTexture);
					glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					glBindTexture(GL_TEXTURE_2D, 0);
					m_colorBuffers.push_back(newTexture);
				}
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, m_colorBuffers[index], 0);
				CheckFramebufferStatus();
				break;
			case FramebufferAttachment::Depth:
				glGenTextures(1, &m_depthBuffer);
				glBindTexture(GL_TEXTURE_2D, m_depthBuffer);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
				glBindTexture(GL_TEXTURE_2D, 0);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthBuffer, 0);
				break;
			case FramebufferAttachment::DepthStencil:
				glGenTextures(1, &m_depthStencilBuffer);
				glBindTexture(GL_TEXTURE_2D, m_depthStencilBuffer);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_width, m_height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);
				glBindTexture(GL_TEXTURE_2D, 0);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthStencilBuffer, 0);
				break;
			case FramebufferAttachment::Stencil:
				glGenTextures(1, &m_stencilBuffer);
				glBindTexture(GL_TEXTURE_2D, m_stencilBuffer);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_STENCIL_INDEX, m_width, m_height, 0, GL_STENCIL_INDEX, GL_UNSIGNED_BYTE, nullptr);
				glBindTexture(GL_TEXTURE_2D, 0);
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
				glDeleteTextures(GL_TEXTURE_2D, &m_colorBuffers[index]);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, 0, 0);
				break;
			case FramebufferAttachment::Depth:
				glDeleteTextures(GL_TEXTURE_2D, &m_depthBuffer);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0);
				break;
			case FramebufferAttachment::DepthStencil:
				glDeleteTextures(GL_TEXTURE_2D, &m_depthStencilBuffer);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, 0, 0);
				break;
			case FramebufferAttachment::Stencil:
				glDeleteTextures(GL_TEXTURE_2D, &m_stencilBuffer);
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, 0, 0);
				break;
			default:
				AE_LOG_FATAL("OpenGLFrameBuffer::Detach -> Error with type");
				break;
		}

		Unbind();
	}
}