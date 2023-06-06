#include "OpenGLGraphicsContext.h"
#include "AEngine/Core/Logger.h"
#include "AEngine/Core/Window.h"
#include <glad/glad.h>

namespace AEngine
{
	OpenGLGraphicsContext::OpenGLGraphicsContext(const Window* window)
	{
		MakeCurrent(window);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			AE_LOG_FATAL("OpenGLGraphicsContext::Constructor::Failed");
		}
	}

	void OpenGLGraphicsContext::MakeCurrent(const Window* window)
	{
		glfwMakeContextCurrent(GetGLFWWindow(window));
	}

	void OpenGLGraphicsContext::SwapBuffers(const Window* window)
	{
		glfwSwapBuffers(GetGLFWWindow(window));
	}

	void OpenGLGraphicsContext::ShowCursor(const Window* window, bool toggle)
	{
		toggle ?
			glfwSetInputMode(GetGLFWWindow(window), GLFW_CURSOR, GLFW_CURSOR_NORMAL) :
			glfwSetInputMode(GetGLFWWindow(window), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	bool OpenGLGraphicsContext::IsShowingCursor(const Window* window)
	{
		return (glfwGetInputMode(GetGLFWWindow(window), GLFW_CURSOR) == GLFW_CURSOR_NORMAL);
	}

	GLFWwindow* OpenGLGraphicsContext::GetGLFWWindow(const Window* window)
	{
		return static_cast<GLFWwindow*>(window->GetNative());
	}
}
