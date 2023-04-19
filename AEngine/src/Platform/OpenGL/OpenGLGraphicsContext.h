#pragma once
#include <glfw/glfw3.h>
#include "AEngine/Render/GraphicsContext.h"

namespace AEngine
{
	class OpenGLGraphicsContext : public GraphicsContext
	{
	public:
		OpenGLGraphicsContext(void* window);
		virtual ~OpenGLGraphicsContext() = default;
		void SwapBuffers() override;
		void ShowCursor(bool toggle) override;

	private:
		void Init();
		GLFWwindow* m_context;
	};
}
