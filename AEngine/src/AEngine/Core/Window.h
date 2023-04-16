/**
 * @file
 * @author Christien Alden (34119981)
 * @brief Window interface
**/
#pragma once
#include <string>
#include <memory>
#include "Input.h"

namespace AEngine
{
		/**
		 * @struct WindowProps
		 * @brief Holds initial properties of window
		**/
	struct WindowProps
	{
		WindowProps() = default;
		WindowProps(const std::string& title, int width, int height)
			: title(title), width(width), height(height) {}

		std::string title{"AEngine"};
		int width{1600};
		int height{900};
	};

		/**
		 * @class Window
		 * @brief Window Interface
		**/
	class Window
	{
	public:
		virtual ~Window() = default;
			/**
			 * @brief Returns native window
			 * @retval void*
			**/
		virtual void* GetNative() const = 0;

			/**
			 * @brief Returns the InputQuery object
			 * @returns InputQuery&
			**/
		virtual InputQuery& GetInput() const = 0;

			/**
			 * @brief Query framebuffer size
			 * @param[out] width of framebuffer
			 * @param[out] height of framebuffer
			 * @retval void
			 *
			 * Pass nullptr for either parameter if not needed
			**/
		virtual void GetSize(int *width, int *height) const = 0;

		/**
			 * @brief Runtime update of window
			 * @retval void
			**/
		virtual void OnUpdate() const = 0;
			/**
			 * @brief Creates a new window
			 * @param[in] props initial properties of window
			 * @retval unique_ptr<Window>
			**/
		static std::unique_ptr<Window> Create(const WindowProps& props = WindowProps());
	};
}
