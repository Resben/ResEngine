/**
 * \file
 * \author Christien Alden (34119981)
*/
#pragma once
#include "AEngine/Render/RenderContextImpl.h"
#include <glfw/glfw3.h>

namespace AEngine
{
	class Window;

	class OpenGLRenderContext : public RenderContextImpl
	{
	public:
			/**
			 * \param[in] window The window to create the graphics context with
			*/
		OpenGLRenderContext(const Window* window);
		virtual ~OpenGLRenderContext() = default;
			/**
			 * \copydoc RenderContext::MakeCurrent
			*/
		void MakeCurrent(const Window* window) override;
			/**
			 * \copydoc RenderContext::SwapBuffers
			*/
		void SwapBuffers(const Window* window) override;
			/**
			 * \copydoc RenderContext::ShowCursor
			*/
		void ShowCursor(const Window* window, bool toggle) override;
			/**
			 * \copydoc RenderContext::IsShowingCursor
			*/
		bool IsShowingCursor(const Window* window) override;

	private:
			/**
			 * \brief Gets the native window handle
			 * \param[in] window The window to get the native window handle of
			 * \return The native window handle, i.e., the GLFWwindow pointer
			*/
		GLFWwindow* GetGLFWWindow(const Window* window);
	};
}
