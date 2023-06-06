#pragma once
#include "AEngine/Render/GraphicsContextImpl.h"
#include <glfw/glfw3.h>

namespace AEngine
{
	class Window;

	class OpenGLGraphicsContext : public GraphicsContextImpl
	{
	public:
			/**
			 * \param[in] window The window to create the graphics context for
			*/
		OpenGLGraphicsContext(const Window* window);
		virtual ~OpenGLGraphicsContext() = default;
		void MakeCurrent(const Window* window) override;
		void SwapBuffers(const Window* window) override;
		void ShowCursor(const Window* window, bool toggle) override;
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
