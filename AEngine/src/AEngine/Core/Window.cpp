/**
 * \file
 * \author Christien Alden (34119981)
*/
#include "Window.h"
#include "Logger.h"
#include "AEngine/Events/KeyEvent.h"
#include "AEngine/Events/MouseEvent.h"
#include "AEngine/Events/ApplicationEvent.h"

#ifdef AE_PLATFORM_WINDOWS
	#include "Platform/Windows/WindowsWindow.h"
#endif

namespace AEngine
{
	Window::Window(Properties properties)
		: m_properties{ properties }, m_eventHandler{ }
	{
		AE_LOG_INFO("Window::Constructor");

		// // register the window events
		RegisterEventHandler<WindowResized>(0, [&, this](WindowResized& event) -> bool {
			m_properties.width = event.GetWidth();
			m_properties.height = event.GetHeight();
			return false;
		});
	}

	void Window::PostEvent(UniquePtr<Event> event)
	{
		m_eventHandler.PushEvent(std::move(event));
	}

	void Window::SetKeyState(AEKey key, AEInputState state)
	{
		if (key == AEKey::INVALID)
		{
			return;
		}

		m_keyState[static_cast<Size_t>(key)] = state;
	}

	void Window::SetMouseButtonState(AEMouse button, AEInputState state)
	{
		if (button == AEMouse::INVALID)
		{
			return;
		}

		m_mouseState[static_cast<Size_t>(button)] = state;
	}

	void Window::SetMousePosition(const Math::vec2 &position)
	{
		m_mousePosition = position;
	}

	void Window::SetMouseScroll(const Math::vec2 &scroll)
	{
		m_mouseScroll = scroll;
	}

	void Window::SwapInputBuffers()
	{
		m_keyStateLast = m_keyState;
		m_mouseStateLast = m_mouseState;
		m_mousePositionLast = m_mousePosition;
		m_mouseScrollLast = m_mouseScroll;
	}

	void Window::OnUpdateInput()
	{
		// check for updated key events
		Size_t keyArrSize = m_keyState.size();
		for (Size_t i = 0; i < keyArrSize; ++i)
		{
			// newly pressed key
			if (m_keyState[i] == AEInputState::Pressed && m_keyStateLast[i] == AEInputState::Released)
			{
				m_eventHandler.PushEvent(MakeUnique<KeyPressed>(static_cast<AEKey>(i)));
			}

			// key was pressed in the last frame
			else if (m_keyState[i] == AEInputState::Pressed && m_keyStateLast[i] == AEInputState::Pressed)
			{
				m_eventHandler.PushEvent(MakeUnique<KeyRepeated>(static_cast<AEKey>(i)));
			}

			// key was just released, i.e., it was either pressed or held
			else if (m_keyState[i] == AEInputState::Released && m_keyStateLast[i] != AEInputState::Released)
			{
				m_eventHandler.PushEvent(MakeUnique<KeyReleased>(static_cast<AEKey>(i)));
			}
		}

		// check for updated mouse button events
		Size_t mouseArrSize = m_mouseState.size();
		for (Size_t i = 0; i < mouseArrSize; ++i)
		{
			// newly pressed button
			if (m_mouseState[i] == AEInputState::Pressed && m_mouseStateLast[i] == AEInputState::Released)
			{
				m_eventHandler.PushEvent(MakeUnique<MousePressed>(static_cast<AEMouse>(i)));
			}

			// button was pressed in the last frame
			else if (m_mouseState[i] == AEInputState::Pressed && m_mouseStateLast[i] == AEInputState::Pressed)
			{
				m_eventHandler.PushEvent(MakeUnique<MouseRepeated>(static_cast<AEMouse>(i)));
			}

			// button was just released, i.e., it was either pressed or held
			else if (m_mouseState[i] == AEInputState::Released && m_mouseStateLast[i] != AEInputState::Released)
			{
				m_eventHandler.PushEvent(MakeUnique<MouseReleased>(static_cast<AEMouse>(i)));
			}
		}

		// fire off mouse moved and mouse scrolled events
		/// \remark We could look at only firing events when they change, but we would have to fire first the zero delta event
		Math::vec2 delta = m_mousePosition - m_mousePositionLast;
		m_eventHandler.PushEvent(MakeUnique<MouseMoved>(m_mousePosition, delta));
		m_eventHandler.PushEvent(MakeUnique<MouseScrolled>(m_mouseScroll));
	}

	UniquePtr<Window> Window::Create(const Properties &properties)
	{
		AE_LOG_INFO("Window::Create");

#ifdef AE_PLATFORM_WINDOWS
		return MakeUnique<WindowsWindow>(properties);
#else
		AE_LOG_FATAL("Platform not implemented");
#endif
	}

	Math::uvec2 Window::GetSize() const
	{
		return { m_properties.width, m_properties.height };
	}

	void Window::MakeCurrent() const
	{
		RenderContext::MakeCurrent(this);
	}

	void Window::SwapBuffers() const
	{
		RenderContext::SwapBuffers(this);
	}

	void Window::ShowCursor(bool toggle) const
	{
		RenderContext::ShowCursor(this, toggle);
	}

	bool Window::IsShowingCursor() const
	{
		return RenderContext::IsShowingCursor(this);
	}

	const std::string &Window::GetTitle() const
	{
		return m_properties.title;
	}

	bool Window::IsVisible() const
	{
		return m_properties.visible;
	}
}
