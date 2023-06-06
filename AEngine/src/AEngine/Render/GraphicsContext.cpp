/**
 * \file
 * \author Christien Alden (34119981)
*/
#include "GraphicsContext.h"
#include "GraphicsContextImpl.h"

namespace AEngine
{
	UniquePtr<GraphicsContextImpl> GraphicsContext::s_impl{ nullptr };
	bool GraphicsContext::s_initialised{ false };

	void GraphicsContext::Initialise(const Window* window, WindowAPI api)
	{
		if (s_initialised)
		{
			return;
		}

		s_impl = GraphicsContextImpl::Create(window, api);
		s_initialised = true;
	}

	void GraphicsContext::MakeCurrent(const Window* window)
	{
		s_impl->MakeCurrent(window);
	}

	void GraphicsContext::SwapBuffers(const Window* window)
	{
		s_impl->SwapBuffers(window);
	}

	void GraphicsContext::ShowCursor(const Window* window, bool toggle)
	{
		s_impl->ShowCursor(window, toggle);
	}

	bool GraphicsContext::IsShowingCursor(const Window* window)
	{
		return s_impl->IsShowingCursor(window);
	}
}
