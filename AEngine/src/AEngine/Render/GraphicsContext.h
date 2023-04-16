#pragma once
#include <memory>
#include <glad/glad.h>

namespace AEngine
{
	class GraphicsContext
	{
	public:
		virtual ~GraphicsContext() = default;
		virtual void SwapBuffers() = 0;
		virtual void Init() = 0;
		
		static std::unique_ptr<GraphicsContext> Create(void *window);
	};
}
