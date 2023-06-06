#pragma once
#include "AEngine/Core/Types.h"
#include "AEngine/Core/Window.h"

namespace AEngine
{
	class GraphicsContext;

	class GraphicsContextImpl
	{
	public:
		virtual ~GraphicsContextImpl() = default;
			/**
			 * \copydoc GraphicsContext::MakeCurrent
			*/
		virtual void MakeCurrent(const Window* window) = 0;
			/**
			 * \copydoc GraphicsContext::SwapBuffers
			*/
		virtual void SwapBuffers(const Window* window) = 0;
			/**
			 * \copydoc GraphicsContext::ShowCursor
			*/
		virtual void ShowCursor(const Window* window, bool toggle = true) = 0;
			/**
			 * \copydoc GraphicsContext::IsShowingCursor
			*/
		virtual bool IsShowingCursor(const Window* window) = 0;

			/**
			 * \brief Creates a graphics context
			 * \param[in] window The window to create the graphics context for
			 * \param[in] api The window API to use
			 * \return A unique pointer to the graphics context
			*/
		static UniquePtr<GraphicsContextImpl> Create(const Window* window, WindowAPI api);
	};
}
