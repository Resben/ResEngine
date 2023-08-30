#include "Framebuffer.h"
#include "AEngine/Core/Logger.h"
#include "Platform/OpenGL/OpenGLFramebuffer.h"
#include "RenderCommand.h"

namespace AEngine
{
    SharedPtr<Framebuffer> Framebuffer::Create()
	{
		switch (RenderCommand::GetLibrary())
		{
		case RenderLibrary::OpenGL:
			return MakeShared<OpenGLFramebuffer>();
		default:
			AE_LOG_FATAL("VertexArray::Create::Error -> Invalid RenderLibrary");
		}
	}
}