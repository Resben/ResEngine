#include "Framebuffer.h"
#include "AEngine/Core/Logger.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"
#include "RenderCommand.h"

namespace AEngine
{
	Framebuffer::~Framebuffer() {}

    SharedPtr<Framebuffer> Framebuffer::Create(Math::uvec2 windowSize)
	{
		switch (RenderCommand::GetLibrary())
		{
		case RenderLibrary::OpenGL:
			return MakeShared<OpenGLFramebuffer>(windowSize);
		default:
			AE_LOG_FATAL("VertexArray::Create::Error -> Invalid RenderLibrary");
		}
	}
}