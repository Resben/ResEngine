/**
 * \file
 * \author Christien Alden (34119981)
*/
#pragma once
#include "AEngine/Core/Types.h"
#include "AEngine/Core/Window.h"

namespace AEngine
{
	class GraphicsContextImpl;

	class GraphicsContext
	{
	public:
			/**
			 * \brief Initialises a graphics context
			 * \param[in] window The window to initialise the graphics context for
			 * \param[in] api The window API to use
			*/
		static void Initialise(const Window* window, WindowAPI api);
					/**
			 * \brief Selects the window for rendering
			 * \param[in] window The window to render to
			*/
		static void MakeCurrent(const Window* window);
			/**
			 * \brief Swaps the buffers of a window
			 * \param[in] window The window to swap the buffers of
			*/
		static void SwapBuffers(const Window* window);
			/**
			 * \brief Shows or hides the cursor
			 * \param[in] window The window to show or hide the cursor of
			 * \param[in] toggle True to show the cursor, false to hide it, default is true
			*/
		static void ShowCursor(const Window* window, bool toggle = true);
			/**
			 * \brief Checks if the cursor is showing
			 * \param[in] window The window to check the cursor of
			 * \retval True The cursor is showing
			 * \retval False The cursor is not showing
			*/
		static bool IsShowingCursor(const Window* window);

	private:
		static UniquePtr<GraphicsContextImpl> s_impl;
		static bool s_initialised;
	};
}
