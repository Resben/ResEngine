#include "OpenGLGraphicsContext.h"
#include "AEngine/Core/Logger.h"

#include <glad/glad.h>

namespace AEngine
{
	OpenGLGraphicsContext::OpenGLGraphicsContext(void* window)
		: m_context(static_cast<GLFWwindow*>(window))
	{
		Init();
	}

	void OpenGLGraphicsContext::SwapBuffers()
	{
		glfwSwapBuffers(m_context);
	}

	void OpenGLGraphicsContext::Init()
	{
		glfwMakeContextCurrent(m_context);
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			AE_LOG_FATAL("OpenGLGraphicsContext::Init::Failed");
		}
	}

	void OpenGLGraphicsContext::ShowCursor(bool toggle)
	{
		if (toggle)
		{
			glfwSetInputMode(m_context, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		else
		{
			glfwSetInputMode(m_context, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
	}
}
