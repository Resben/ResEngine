#pragma once
#include <memory>
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
		
		static std::unique_ptr<GraphicsContext> Create(void *window, WindowAPI api);
	};
}
