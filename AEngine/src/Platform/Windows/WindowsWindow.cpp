/**
 * @file
 * @author Christien Alden (34119981)
 * @brief Windows specific window
**/
#include "WindowsWindow.h"
#include "AEngine/Core/Application.h"
#include "AEngine/Events/EventHandler.h"
#include "AEngine/Core/Logger.h"
#include "AEngine/Render/RenderCommand.h"
#include "WindowsKeys.h"

namespace AEngine
{
	Size_t WindowsWindow::s_windowCount{ 0 };

	WindowsWindow::~WindowsWindow()
	{
		AE_LOG_DEBUG("WindowsWindow::Destructor");
		glfwDestroyWindow(m_native);
	}

	void WindowsWindow::Maximise()
	{
		AE_LOG_DEBUG("WindowsWindow::Maximisze");
		glfwMaximizeWindow(m_native);
	}

	void WindowsWindow::Minimise()
	{
		AE_LOG_DEBUG("WindowsWindow::Minimisze");
		glfwIconifyWindow(m_native);
	}

	void WindowsWindow::SetVisible(bool toggle)
	{
		AE_LOG_DEBUG("WindowsWindow::SetVisible");
		m_properties.visible = toggle;
		toggle ? glfwShowWindow(m_native) : glfwHideWindow(m_native);
	}

	void WindowsWindow::SetPosition(std::optional<unsigned int> xpos, std::optional<unsigned int> ypos) const
	{
		/// \todo fix this as the window is rendering off screen
		AE_LOG_ERROR("WindowsWindow::SetPosition::NotImplemented");

		// get the existing positions if not specified
		// Math::uvec2 pos = GetPosition();
		// glfwSetWindowPos(m_native, xpos.value_or(pos.x), ypos.value_or(pos.y));
	}

	Math::uvec2 WindowsWindow::GetPosition() const
	{
		AE_LOG_DEBUG("WindowsWindow::GetPosition");
		int xpos, ypos;
		glfwGetWindowPos(m_native, &xpos, &ypos);
		return { xpos, ypos };
	}

	WindowsWindow::WindowsWindow(const Properties& props)
		: Window(props)
	{
		if (s_windowCount == 0)
		{
			InitialiseGLFW();
			++s_windowCount;
		}
		else // activated upon second create window call
		{
			AE_LOG_ERROR("WindowsWindow::Constructor::Failed -> Only supports one window");
			glfwTerminate();
			exit(1);
		}

		// create GLFW window with initial properties
		m_native = glfwCreateWindow(m_properties.width, m_properties.height, m_properties.title.c_str(), NULL, NULL);
		if (m_native == nullptr)
		{
			AE_LOG_ERROR("WindowsWindow::GLFW::Window::Create::Failed");
			glfwTerminate();
			exit(1);
		}

		// attach graphics context to glfw window
		RenderContext::Initialise(this, WindowAPI::GLFW);

		// pass the pointer to this window to the event handler
		glfwSetWindowUserPointer(m_native, this);
		glfwSetKeyCallback(m_native, [](GLFWwindow* context, int key, int scancode, int action, int mods) {
			Window* window = static_cast<Window*>(glfwGetWindowUserPointer(context));
			AEKey aeKey = ToAEKey(key);
			if (action == GLFW_PRESS)
			{
				window->SetKeyState(aeKey, AEInputState::Pressed);
			}
			else if (action == GLFW_RELEASE)
			{
				window->SetKeyState(aeKey, AEInputState::Released);
			}
		});

		glfwSetCharCallback(m_native, [](GLFWwindow* context, unsigned int codepoint) {
			Window* window = static_cast<Window*>(glfwGetWindowUserPointer(context));
			window->PostEvent(MakeUnique<KeyTyped>(codepoint));
		});

		glfwSetCursorPosCallback(m_native, [](GLFWwindow* context, double xpos, double ypos) {
			Window* window = static_cast<Window*>(glfwGetWindowUserPointer(context));
			window->SetMousePosition({ xpos, ypos });
		});

		glfwSetMouseButtonCallback(m_native, [](GLFWwindow* context, int button, int action, int mods) {
			Window* window = static_cast<Window*>(glfwGetWindowUserPointer(context));
			AEMouse aeMouse = ToAEMouse(button);
			if (action == GLFW_PRESS)
			{
				window->SetMouseButtonState(aeMouse, AEInputState::Pressed);
			}
			else if (action == GLFW_RELEASE)
			{
				window->SetMouseButtonState(aeMouse, AEInputState::Released);
			}
		});

		glfwSetScrollCallback(m_native, [](GLFWwindow* context, double xoffset, double yoffset) {
			Window* window = static_cast<Window*>(glfwGetWindowUserPointer(context));
			window->SetMouseScroll({ xoffset, yoffset });
		});

		glfwSetWindowCloseCallback(m_native, [](GLFWwindow* context) {
			Window* window = static_cast<Window*>(glfwGetWindowUserPointer(context));
			window->PostEvent(MakeUnique<WindowClosed>());
		});

		glfwSetWindowSizeCallback(m_native, [](GLFWwindow* context, int width, int height) {
			Window* window = static_cast<Window*>(glfwGetWindowUserPointer(context));
			window->PostEvent(MakeUnique<WindowResized>(width, height));
		});

		MakeCurrent();
	}

	void* WindowsWindow::GetNative() const
	{
		return m_native;
	}

//--------------------------------------------------------------------------------
	void WindowsWindow::SetTitle(const std::string& title)
	{
		m_properties.title = title;
		glfwSetWindowTitle(m_native, title.c_str());
	}

	void WindowsWindow::SetSize(std::optional<unsigned int> width, std::optional<unsigned int> height)
	{
		m_properties.width = width.has_value() ? width.value() : m_properties.width;
		m_properties.height = height.has_value() ? height.value() : m_properties.height;
		glfwSetWindowSize(m_native, m_properties.width, m_properties.height);
	}

//--------------------------------------------------------------------------------
	void WindowsWindow::OnUpdate()
	{
		// swap the input buffers on the window, then poll events to update the input state
		SwapInputBuffers();
		glfwPollEvents();

		// then check for any repeated keys which will fire a new event (repeated)
		OnUpdateInput();

		// finally swap the buffers and clear the screen
		SwapBuffers();
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
	}
}
