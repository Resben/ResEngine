/**
 * @file
 * @author Christien Alden (34119981)
 * @brief Windows specific window
**/
#include "AEngine/Core/Logger.h"
#include "WindowsWindow.h"
#include "WindowsKeys.h"
#include "WindowsInput.h"
#include "AEngine/Events/ApplicationEvent.h"
#include "AEngine/Events/KeyEvent.h"
#include "AEngine/Events/MouseEvent.h"
#include "AEngine/Events/EventQueue.h"
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

		// set input context
		m_input = new GLFWInput(m_context);

		// set callbacks to integrate with event system
		glfwSetKeyCallback(m_context, [](GLFWwindow* context, int key, int scancode, int action, int mods) {
			if (action == GLFW_PRESS)
			{
				EventQueue::Instance().PushEvent(new KeyPressed(ToAEKey(key)));
			}
			else if (action == GLFW_RELEASE)
			{
				EventQueue::Instance().PushEvent(new KeyReleased(ToAEKey(key)));
			}
		});

		glfwSetCharCallback(m_context, [](GLFWwindow* context, unsigned int codepoint) {
			EventQueue::Instance().PushEvent(new KeyTyped(codepoint));
		});

		glfwSetCursorPosCallback(m_context, [](GLFWwindow* window, double xpos, double ypos) {
			EventQueue::Instance().PushEvent(new MouseMoved(xpos, ypos));
		});

		glfwSetMouseButtonCallback(m_context, [](GLFWwindow* window, int button, int action, int mods) {
			if (action == GLFW_PRESS)
			{
				EventQueue::Instance().PushEvent(new MouseButtonPressed(ToAEMouse(button)));
			}
			else if (action == GLFW_RELEASE)
			{
				EventQueue::Instance().PushEvent(new MouseButtonReleased(ToAEMouse(button)));
			}
		});

		glfwSetScrollCallback(m_context, [](GLFWwindow* window, double xoffset, double yoffset) {
			EventQueue::Instance().PushEvent(new MouseScrolled(xoffset, yoffset));
		});

		glfwSetWindowCloseCallback(m_context, [](GLFWwindow* window) {
			EventQueue::Instance().PushEvent(new WindowClosed());
		});

		glfwSetWindowSizeCallback(m_context, [](GLFWwindow*, int width, int height) {
			EventQueue::Instance().PushEvent(new WindowResized(width, height));
		});

		glEnable(GL_DEPTH_TEST);
		AE_LOG_TRACE("WindowsWindow::Constructor::Success");
	}

	void* WindowsWindow::GetNative() const
	{
		return m_context;
	}

	GLFWInput& WindowsWindow::GetInput() const
	{
		return *m_input;
	}

	void WindowsWindow::GetSize(int *width, int *height) const
	{
		glfwGetFramebufferSize(m_context, width, height);
	}

	void WindowsWindow::OnUpdate() const
	{
		// clean any non handled window events from last frame
		EventQueue::Instance().Clear(EventCategory::Window);

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
