#include "OpenGLGraphicsContext.h"

namespace AEngine
{
	OpenGLGraphicsContext::OpenGLGraphicsContext(void* window)
		: m_context(static_cast<GLFWwindow*>(window))
	{

	}

	OpenGLGraphicsContext~OpenGLGraphicsContext()
	{
		glfwTerminate(m_context);
	}

	void OpenGLGraphicsContext::SwapBuffers()
	{
		glfwSwapBuffers(m_context);
	}

	bool OpenGLGraphicsContext::Init()
	{
		glfwMakeContextCurrent(m_context);
		return gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	}
}
