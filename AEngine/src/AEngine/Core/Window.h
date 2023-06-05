/**
 * \file
 * \author Christien Alden (34119981)
*/
#pragma once
#include "AEngine/Math/Math.h"
#include "Types.h"
#include <string>

namespace AEngine
{
		/**
		 * \enum WindowAPI
		 * \brief AEngine supported windowing APIs
		*/
	enum class WindowAPI
	{
		NONE = 0, GLFW
	};

		/**
		 * \class Window
		 * \brief Window Interface
		*/
	class Window
	{
	public:
			/**
			 * \struct Properties
			 * \brief Holds properties of window
			*/
		struct Properties
		{
			Properties() = default;
			Properties(const std::string& title, unsigned int width, unsigned int height)
				: title{ title }, width{ width }, height{ height } {}

			std::string title{ "Default AEngine Window" };
			unsigned int width{ 1600 };
			unsigned int height{ 900 };
		};

	public:
			/**
			 * \brief Destructor
			*/
		virtual ~Window() = default;

			/**
			 * \brief Runtime update of window
			 * \retval void
			*/
		virtual void OnUpdate() const = 0;

			/**
			 * \brief Returns windowing API's native window
			 * \retval void* to native window
			*/
		virtual void* GetNative() const = 0;
			/**
			 * \brief Returns current size of window
			 * \return Math::vec2 of current size
			*/
		virtual Math::vec2 GetSize() const = 0;

			/**
			 * \brief Creates a new window
			 * \param[in] properties initial properties of window
			 * \retval A UniquePtr to the new window
			*/
		static UniquePtr<Window> Create(const Properties& properties = Properties());

	protected:
			/**
			 * \brief Constructor
			*/
		Window(Properties properties);
			/**
			 * \brief Properties of window
			*/
		Properties m_properties;
	};
}
