/**
 * \file
 * \author Christien Alden (34119981)
*/
#include "RenderCommandImpl.h"
#include "AEngine/Core/Logger.h"
#include "Platform/OpenGL/OpenGLRenderCommand.h"

namespace AEngine
{
	UniquePtr<RenderCommandImpl> RenderCommandImpl::Create(RenderLibrary graphics)
	{
		switch (graphics)
		{
		case RenderLibrary::OpenGL:
			return MakeUnique<OpenGLRenderCommand>();
		default:
			AE_LOG_FATAL("RenderCommandImpl::Create::Error::RenderLibrary -> Does not exist");
		}
	}
}
