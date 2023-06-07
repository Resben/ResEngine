/**
 * \file
 * \author Christien Alden (34119981)
*/
#include "VertexArray.h"
#include "AEngine/Core/Logger.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "RenderCommand.h"

namespace AEngine
{
	SharedPtr<VertexArray> VertexArray::Create()
	{
		switch (RenderCommand::GetLibrary())
		{
		case RenderLibrary::OpenGL:
			return MakeShared<OpenGLVertexArray>();
		default:
			AE_LOG_FATAL("VertexArray::Create::Error -> Invalid RenderLibrary");
		}
	}
}
