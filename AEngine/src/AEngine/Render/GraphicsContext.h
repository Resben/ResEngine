#pragma once
#include "AEngine/Core/Types.h"
#include "AEngine/Core/Window.h"

namespace AEngine
{
	class GraphicsContext
	{
	public:

		virtual ~GraphicsContext() = default;
		virtual void SwapBuffers() = 0;
		virtual void ShowCursor(bool toggle) = 0;
		virtual bool IsShowingCursor() = 0;

		static UniquePtr<GraphicsContext> Create(void *window, WindowAPI api);
	};
}
