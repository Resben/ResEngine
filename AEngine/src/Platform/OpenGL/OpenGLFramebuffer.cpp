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
    OpenGLFramebuffer::OpenGLFramebuffer()
    {
        glGenFramebuffers(1, &m_framebuffer);
    }

    void OpenGLFramebuffer::Bind(FramebufferMode mode = FramebufferMode::ReadWrite) const
    {
        glBindFramebuffer(g_glFramebufferMode[static_cast<int>(mode)], m_framebuffer);
    }

	void OpenGLFramebuffer::Unbind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

	void OpenGLFramebuffer::Attach(FramebufferAttachment type, unsigned int index = 0)
    {
        switch(type)
        {
            case FramebufferAttachment::Color:
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, GL_TEXTURE_2D, 1, 0); //<< Getting texture index
                break;
            case FramebufferAttachment::Depth:
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 2, 0);
                break;
            case FramebufferAttachment::DepthStencil:
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, 3, 0);
                break;
            case FramebufferAttachment::Stencil:
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, 3, 0);
                break;
            default:
                AE_LOG_FATAL("OpenGLFrameBuffer::Attach -> Error with type");
                break;
        }
    }

	void OpenGLFramebuffer::Detach(FramebufferAttachment type, unsigned int index = 0)
    {

    }
}