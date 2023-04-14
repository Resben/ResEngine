/**
 * @file
 * @author Christien Alden (34119981)
 * @brief Specialisation of Window into a GLFWWindow, also initialises Graphics Context
**/
#include "Logger.h"
#include "GLWindow.h"
#include "InputManager.h"
#include <glad/glad.h>

namespace AEngine
{
	bool GLWindow::s_IsInit{false};

	GLWindow::~GLWindow()
	{
		glfwDestroyWindow(m_context);
	}

	GLWindow::GLWindow(const WindowProps& props)
	{
		if (!s_IsInit)
		{
			InitialiseGLFW();
		}
		else // activated upon second create window call
		{
			AE_LOG_ERROR("GLWindow::Constructor::Failed -> Only supports one window");
			glfwTerminate();
			exit(1);
		}

		// create GLFW window
		m_context = glfwCreateWindow(props.width, props.height, props.title.c_str(), NULL, NULL);
		if (m_context == nullptr)
		{
			AE_LOG_ERROR("GLWindow::GLFW::Window::Create::Failed");
			glfwTerminate();
			exit(1);
		}
		glfwMakeContextCurrent(m_context);

		// initialise OpenGL context
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			AE_LOG_ERROR("GLWindow::GLAD::Initialise::Failed");
			glfwTerminate();
			exit(1);
		}

		glEnable(GL_DEPTH_TEST);
		AE_LOG_TRACE("GLWindow::Constructor::Success");
	}

	void* GLWindow::GetNative() const
	{
		return m_context;
	}

	void GLWindow::GetSize(int *width, int *height) const
	{
		glfwGetFramebufferSize(m_context, width, height);
	}

	void GLWindow::OnUpdate() const
	{
		InputManager::OnUpdate();
		glfwPollEvents();
		glfwSwapBuffers(m_context);
	}

	//--------------------------------------------------------------------------------
	void GLWindow::InitialiseGLFW()
	{
		AE_LOG_TRACE("GLWindow::GLFW::Initialise");
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
		s_IsInit = true;
	}
}
