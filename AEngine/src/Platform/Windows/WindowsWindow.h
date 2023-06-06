/**
 * \file
 * \author Christien Alden (34119981)
 * \brief Windows window interface
**/
#pragma once
#include <GLFW/glfw3.h>
#include "AEngine/Core/Window.h"
#include "AEngine/Render/GraphicsContext.h"

namespace AEngine
{
		/**
		 * \class WindowsWindow
		 * \brief GLFW specialisation Window
		**/
	class WindowsWindow : public Window
	{
	public:
			/**
			 * \param[in] props initial settings
			**/
		WindowsWindow(const Properties& props);
			/**
			 * \brief Destroys glfw window context
			*/
		virtual ~WindowsWindow();

			/**
			 * \copydoc Window::Maximise
			*/
		virtual void Maximise() override;
			/**
			 * \copydoc Window::Minimise
			*/
		virtual void Minimise() override;

			/**
		 	 * \copydoc Window::SetVisible
			*/
		virtual void SetVisible(bool toggle) override;
			/**
			 * \copydoc Window::SetPosition
			*/
		virtual void SetPosition(std::optional<unsigned int> xpos, std::optional<unsigned int> ypos) const;
			/**
			 * \copydoc Window::GetPosition
			*/
		virtual Math::uvec2 GetPosition() const;

			/**
			 * \copydoc Window::SetTitle
			*/
		virtual void SetTitle(const std::string& title) override;
			/**
			 * \copydoc Window::SetSize
			*/
		virtual void SetSize(std::optional<unsigned int> width, std::optional<unsigned int> height) override;

	private:
			/**
			 * \brief Pointer to graphics context
			*/
		UniquePtr<GraphicsContext> m_graphics;
			/**
			 * \brief GLFWwindow context
			*/
		GLFWwindow* m_context;
			/**
			 * \brief Status of GLFW initialisation
			*/
		static bool s_IsInit;

			/**
			 * \copydoc Window::GetNative
			*/
		void* GetNative() const override;
			/**
			 * \copydoc Window::OnUpdate
			**/
		void OnUpdate() const override;
			/**
			 * \brief Initialises GLFW
			*/
		static void InitialiseGLFW();
	};
}
