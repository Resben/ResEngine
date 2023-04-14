/**
 * @file
 * @author Christien Alden (34119981)
 * @brief Windows specific window
**/
#include "AEngine/Core/Logger.h"
#include "WindowsWindow.h"
#include <glad/glad.h>

namespace AEngine
{
	bool WindowsWindow::s_IsInit{false};

	WindowsWindow::~WindowsWindow()
	{
		glfwDestroyWindow(m_context);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		if (!s_IsInit)
		{
			InitialiseGLFW();
		}
		else // activated upon second create window call
		{
			AE_LOG_ERROR("WindowsWindow::Constructor::Failed -> Only supports one window");
			glfwTerminate();
			exit(1);
		}

		// create GLFW window
		m_context = glfwCreateWindow(props.width, props.height, props.title.c_str(), NULL, NULL);
		if (m_context == nullptr)
		{
			AE_LOG_ERROR("WindowsWindow::GLFW::Window::Create::Failed");
			glfwTerminate();
			exit(1);
		}
		glfwMakeContextCurrent(m_context);

		// initialise OpenGL context
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			AE_LOG_ERROR("WindowsWindow::GLAD::Initialise::Failed");
			glfwTerminate();
			exit(1);
		}

		// set callbacks to integrate with event system
		glfwSetKeyCallback(m_context, [](GLFWwindow* context, int key, int scancode, int action, int mods) -> void {
			// to implement
			});

		glfwSetCharCallback(m_context, [](GLFWwindow* context, unsigned int codepoint) -> void {
			// to implement
			});

		glfwSetCursorPosCallback(m_context, [](GLFWwindow* window, double xpos, double ypos) -> void {
			// to implement
			});

		glfwSetMouseButtonCallback(m_context, [](GLFWwindow* window, int button, int action, int mods) -> void {
			// to implement
			});

		glfwSetScrollCallback(m_context, [](GLFWwindow* window, double xoffset, double yoffset) -> void {
			// to implement
			});

		glEnable(GL_DEPTH_TEST);
		AE_LOG_TRACE("WindowsWindow::Constructor::Success");
	}

	void* WindowsWindow::GetNative() const
	{
		return m_context;
	}

	void WindowsWindow::GetSize(int *width, int *height) const
	{
		glfwGetFramebufferSize(m_context, width, height);
	}

	void WindowsWindow::OnUpdate() const
	{
		// need to make this rely on GraphicsCommands
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwPollEvents();
		glfwSwapBuffers(m_context);
	}

	//--------------------------------------------------------------------------------
	void WindowsWindow::InitialiseGLFW()
	{
		AE_LOG_TRACE("WindowsWindow::GLFW::Initialise");
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
		s_IsInit = true;
	}
}
