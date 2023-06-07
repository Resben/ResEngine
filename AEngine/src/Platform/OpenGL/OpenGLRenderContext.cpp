/**
 * \file
 * \author Christien Alden (34119981)
*/
#include "OpenGLRenderContext.h"
#include "AEngine/Core/Logger.h"
#include "AEngine/Core/Window.h"
#include <glad/glad.h>

namespace AEngine
{
	OpenGLRenderContext::OpenGLRenderContext(const Window* window)
	{
		MakeCurrent(window);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			AE_LOG_FATAL("OpenGLRenderContext::Constructor::Failed");
		}
	}

	void OpenGLRenderContext::MakeCurrent(const Window* window)
	{
		glfwMakeContextCurrent(GetGLFWWindow(window));
	}

	void OpenGLRenderContext::SwapBuffers(const Window* window)
	{
		glfwSwapBuffers(GetGLFWWindow(window));
	}

	void OpenGLRenderContext::ShowCursor(const Window* window, bool toggle)
	{
		toggle ?
			glfwSetInputMode(GetGLFWWindow(window), GLFW_CURSOR, GLFW_CURSOR_NORMAL) :
			glfwSetInputMode(GetGLFWWindow(window), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	bool OpenGLRenderContext::IsShowingCursor(const Window* window)
	{
		return (glfwGetInputMode(GetGLFWWindow(window), GLFW_CURSOR) == GLFW_CURSOR_NORMAL);
	}

	GLFWwindow* OpenGLRenderContext::GetGLFWWindow(const Window* window)
	{
		return static_cast<GLFWwindow*>(window->GetNative());
	}
}
