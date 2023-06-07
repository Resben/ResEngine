/**
 * \file
 * \author Christien Alden (34119981)
*/
#include "RenderContext.h"
#include "RenderContextImpl.h"

namespace AEngine
{
	UniquePtr<RenderContextImpl> RenderContext::s_impl{ nullptr };
	bool RenderContext::s_initialised{ false };

	void RenderContext::Initialise(const Window* window, WindowAPI api)
	{
		if (s_initialised)
		{
			return;
		}

		s_impl = RenderContextImpl::Create(window, api);
		s_initialised = true;
	}

	void RenderContext::MakeCurrent(const Window* window)
	{
		s_impl->MakeCurrent(window);
	}

	void RenderContext::SwapBuffers(const Window* window)
	{
		s_impl->SwapBuffers(window);
	}

	void RenderContext::ShowCursor(const Window* window, bool toggle)
	{
		s_impl->ShowCursor(window, toggle);
	}

	bool RenderContext::IsShowingCursor(const Window* window)
	{
		return s_impl->IsShowingCursor(window);
	}
}
