/**
 * @file
 * @author Christien Alden (34119981)
 * @brief GLFW window interface
**/
#pragma once
#include "Window.h"
#include <GLFW/glfw3.h>

namespace AEngine
{
		/**
		 * @class GLWindow
		 * @brief GLFW specialisation Window
		**/
	class GLWindow : public Window
	{
	public:
			/**
			 * @brief Initialises window and graphics context
			 * @param[in] props initial settings
			**/
		GLWindow(const WindowProps& props);
			/**
			 * @brief Destroys GLFW window
			**/
		virtual ~GLWindow();

			/**
			 * @brief Returns GLFWwindow* as void*
			 * @return void*
			**/
		void* GetNative() const override;
			/**
			 * @brief Queries GLFW framebuffer
			 * @param[out] width of framebuffer
			 * @param[out] height of framebuffer
			 * @return void
			 *
			 * Can specify nullptr for either parameter if not needed
			**/
		void GetSize(int *width, int *height) const override;
			/**
			 * @brief GLFW runtime update
			 * @return void
			 *
			 * Updates input manager, polls GLFW events, and swaps buffers
			**/
		void OnUpdate() const override;

	private:
		GLFWwindow* m_context;				///< Current window context
		static bool s_IsInit;				///< Status of GLFW initialisation
		static void InitialiseGLFW();		///< Helper to initialise GLFW
	};
}
