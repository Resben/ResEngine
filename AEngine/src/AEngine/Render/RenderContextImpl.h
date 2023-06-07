/**
 * \file
 * \author Christien Alden (34119981)
*/
#pragma once
#include "AEngine/Core/Types.h"
#include "AEngine/Core/Window.h"

namespace AEngine
{
	class RenderContext;
		/**
		 * \class RenderContextImpl
		 * \brief Separates the internal implementation of a render context from the interface
		*/
	class RenderContextImpl
	{
	public:
		virtual ~RenderContextImpl() = default;
			/**
			 * \copydoc RenderContext::MakeCurrent
			*/
		virtual void MakeCurrent(const Window* window) = 0;
			/**
			 * \copydoc RenderContext::SwapBuffers
			*/
		virtual void SwapBuffers(const Window* window) = 0;
			/**
			 * \copydoc RenderContext::ShowCursor
			*/
		virtual void ShowCursor(const Window* window, bool toggle = true) = 0;
			/**
			 * \copydoc RenderContext::IsShowingCursor
			*/
		virtual bool IsShowingCursor(const Window* window) = 0;

			/**
			 * \brief Creates a graphics context
			 * \param[in] window The window to create the graphics context for
			 * \param[in] api The window API to use
			 * \return A unique pointer to the graphics context
			*/
		static UniquePtr<RenderContextImpl> Create(const Window* window, WindowAPI api);
	};
}
