/**
 * @file
 * @author Christien Alden (34119981)
 * @brief Window interface
**/
#pragma once
#include <memory>
#include <string>
#include "AEngine/Math/Math.hpp"
#include "Input.h"

namespace AEngine
{
	enum class WindowAPI
	{
		NONE = 0, GLFW
	};

		/**
		 * @struct WindowProps
		 * @brief Holds properties of window
		**/
	struct WindowProps
	{
		WindowProps() = default;
		WindowProps(const std::string& title, unsigned int width, unsigned int height)
			: title(title), width(width), height(height) {}

		std::string title{ "AEngine" };
		unsigned int width{ 1600 };
		unsigned int height{ 900 };
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
			 * @brief Returns current size of window
			 * @return Math::vec2 containins current size
			**/
		virtual Math::vec2 GetSize() const = 0;

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

	protected:
		Window(WindowProps props);
		WindowProps m_props;		///< Properties of window
	};
}
