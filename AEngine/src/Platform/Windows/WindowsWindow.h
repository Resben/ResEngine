/**
 * @file
 * @author Christien Alden (34119981)
 * @brief Windows window interface
**/
#pragma once
#include <GLFW/glfw3.h>
#include "AEngine/Core/Window.h"
#include "AEngine/Render/GraphicsContext.h"

namespace AEngine
{
		/**
		 * @class WindowsWindow
		 * @brief GLFW specialisation Window
		**/
	class WindowsWindow : public Window
	{
	public:
			/**
			 * @brief Initialises window and graphics context
			 * @param[in] props initial settings
			**/
		WindowsWindow(const Properties& props);
			/**
			 * @brief Destroys GLFW window
			**/
		virtual ~WindowsWindow();

			/**
			 * @brief Returns GLFWwindow* as void*
			 * @return void*
			**/
		void* GetNative() const override;

			/**
			 * @brief Returns current size of window
			 * @return Math::vec2 containins current size
			**/
		Math::vec2 GetSize() const override;

			/**
			 * @brief GLFW runtime update
			 * @return void
			 *
			 * Updates input manager, polls GLFW events, and swaps buffers
			**/
		void OnUpdate() const override;

	private:

		UniquePtr<GraphicsContext> m_graphics;
		GLFWwindow* m_context;				///< Current window context
		static bool s_IsInit;				///< Status of GLFW initialisation
		static void InitialiseGLFW();		///< Helper to initialise GLFW
	};
}
