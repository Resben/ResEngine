/**
 * \file
 * \author Christien Alden (34119981)
*/
#include "Buffer.h"
#include "AEngine/Core/Logger.h"
#include "AEngine/Render/RenderCommand.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

namespace AEngine
{
	SharedPtr<Buffer> Buffer::Create(BufferType type)
	{
		switch (RenderCommand::GetLibrary())
		{
		case GraphicsLibrary::OpenGL:
			return MakeShared<OpenGLBuffer>(type);
		default:
			AE_LOG_FATAL("Buffer::Create::RenderLibrary::Error -> None selected");
		}
	}
}
