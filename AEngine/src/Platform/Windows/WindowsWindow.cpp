/**
 * @file
 * @author Christien Alden (34119981)
 * @brief Windows specific window
**/
#include "WindowsWindow.h"
#include "AEngine/Core/Application.h"
#include "AEngine/Core/Logger.h"
#include "AEngine/Events/ApplicationEvent.h"
#include "AEngine/Events/EventQueue.h"
#include "AEngine/Events/KeyEvent.h"
#include "AEngine/Events/MouseEvent.h"
#include "AEngine/Input/InputImpl.h"
#include "AEngine/Render/RenderCommand.h"
#include "WindowsKeys.h"

namespace AEngine
{
	bool WindowsWindow::s_IsInit{false};

	WindowsWindow::~WindowsWindow()
	{
		AE_LOG_DEBUG("WindowsWindow::Destructor");
		glfwDestroyWindow(m_context);
	}

	void WindowsWindow::Maximise()
	{
		AE_LOG_DEBUG("WindowsWindow::Maximisze");
		glfwMaximizeWindow(m_context);
	}

	void WindowsWindow::Minimise()
	{
		AE_LOG_DEBUG("WindowsWindow::Minimisze");
		glfwIconifyWindow(m_context);
	}

	void WindowsWindow::SetVisible(bool toggle)
	{
		AE_LOG_DEBUG("WindowsWindow::SetVisible");
		m_properties.visible = toggle;
		toggle ? glfwShowWindow(m_context) : glfwHideWindow(m_context);
	}

	void WindowsWindow::SetPosition(std::optional<unsigned int> xpos, std::optional<unsigned int> ypos) const
	{
		/// \todo fix this as the window is rendering off screen
		return;

		// get the existing positions if not specified
		Math::uvec2 pos = GetPosition();
		glfwSetWindowPos(m_context, xpos.value_or(pos.x), ypos.value_or(pos.y));
	}

	Math::uvec2 WindowsWindow::GetPosition() const
	{
		AE_LOG_DEBUG("WindowsWindow::GetPosition");
		int xpos, ypos;
		glfwGetWindowPos(m_context, &xpos, &ypos);
		return { xpos, ypos };
	}

	WindowsWindow::WindowsWindow(const Properties& props)
		: Window(props)
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

		// create GLFW window with initial properties
		m_context = glfwCreateWindow(m_properties.width, m_properties.height, m_properties.title.c_str(), NULL, NULL);
		if (m_context == nullptr)
		{
			AE_LOG_ERROR("WindowsWindow::GLFW::Window::Create::Failed");
			glfwTerminate();
			exit(1);
		}

		// attach graphics context to glfw window
		m_graphics = GraphicsContext::Create(m_context, WindowAPI::GLFW);

		// set callbacks to integrate with event system
		glfwSetWindowUserPointer(m_context, &m_properties);
		glfwSetKeyCallback(m_context, [](GLFWwindow* context, int key, int scancode, int action, int mods) {
			if (action == GLFW_PRESS)
			{
				// buffer state into array then push event
				AEKey keycode = ToAEKey(key);
				InputImpl::Instance().SetKeyState(keycode, true);
				EventQueue::Instance().PushEvent(MakeUnique<KeyPressed>(keycode));
			}
			else if (action == GLFW_RELEASE)
			{
				AEKey keycode = ToAEKey(key);
				InputImpl::Instance().SetKeyState(keycode, false);
				EventQueue::Instance().PushEvent(MakeUnique<KeyReleased>(keycode));
			}
		});

		glfwSetCharCallback(m_context, [](GLFWwindow* context, unsigned int codepoint) {
			EventQueue::Instance().PushEvent(MakeUnique<KeyTyped>(codepoint));
		});

		glfwSetCursorPosCallback(m_context, [](GLFWwindow* window, double xpos, double ypos) {
			Math::vec2 pos{ xpos, ypos };
			InputImpl::Instance().SetMousePosition(pos);
			EventQueue::Instance().PushEvent(MakeUnique<MouseMoved>(pos));
		});

		glfwSetMouseButtonCallback(m_context, [](GLFWwindow* window, int button, int action, int mods) {
			if (action == GLFW_PRESS)
			{
				AEMouse mouse = ToAEMouse(button);
				InputImpl::Instance().SetMouseButtonState(mouse, true);
				EventQueue::Instance().PushEvent(MakeUnique<MouseButtonPressed>(mouse));
			}
			else if (action == GLFW_RELEASE)
			{
				AEMouse mouse = ToAEMouse(button);
				InputImpl::Instance().SetMouseButtonState(mouse, false);
				EventQueue::Instance().PushEvent(MakeUnique<MouseButtonReleased>(mouse));
			}
		});

		glfwSetScrollCallback(m_context, [](GLFWwindow* window, double xoffset, double yoffset) {
			Math::vec2 scroll{ xoffset, yoffset };
			InputImpl::Instance().SetMouseScroll(scroll);
			EventQueue::Instance().PushEvent(MakeUnique<MouseScrolled>(scroll));
		});

		glfwSetWindowCloseCallback(m_context, [](GLFWwindow* window) {
			EventQueue::Instance().PushEvent(MakeUnique<WindowClosed>());
		});

		glfwSetWindowSizeCallback(m_context, [](GLFWwindow* window, int width, int height) {
			Properties* props = static_cast<Properties*>(glfwGetWindowUserPointer(window));
			props->width = static_cast<unsigned int>(width);
			props->height = static_cast<unsigned int>(height);
			EventQueue::Instance().PushEvent(MakeUnique<WindowResized>(width, height));
		});

		/// \todo provide a way to set cursor mode outside here
		this->m_graphics->ShowCursor(false);
	}

	void* WindowsWindow::GetNative() const
	{
		return m_context;
	}

//--------------------------------------------------------------------------------
	void WindowsWindow::SetTitle(const std::string& title)
	{
		m_properties.title = title;
		glfwSetWindowTitle(m_context, title.c_str());
	}

	void WindowsWindow::SetSize(std::optional<unsigned int> width, std::optional<unsigned int> height)
	{
		m_properties.width = width.has_value() ? width.value() : m_properties.width;
		m_properties.height = height.has_value() ? height.value() : m_properties.height;
		glfwSetWindowSize(m_context, m_properties.width, m_properties.height);
	}

//--------------------------------------------------------------------------------
	void WindowsWindow::OnUpdate() const
	{
		InputImpl::Instance().OnUpdate();
		EventQueue::Instance().Clear(EventCategory::Window);
		glfwPollEvents();
		m_graphics->SwapBuffers();
		RenderCommand::Clear();
	}

//--------------------------------------------------------------------------------
	void WindowsWindow::InitialiseGLFW()
	{
		AE_LOG_INFO("WindowsWindow::GLFW::Initialise");
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
		s_IsInit = true;
	}
}
