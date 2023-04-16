#pragma once
#include <glfw/glfw3.h>
#include "AEngine/Render/GraphicsContext.h"

namespace AEngine
{
	class OpenGLGraphicsContext : public GraphicsContext
	{
	public:
		OpenGLGraphicsContext(void* window);
		virtual ~OpenGLGraphicsContext();
		void SwapBuffers() override;
		bool Init() override;

	private:
		GLFWwindow* m_context;
	};
}
