#include "InputImpl.h"

namespace AEngine
{
//--------------------------------------------------------------------------------
// Frontend functions -> exposed by Input.h
//--------------------------------------------------------------------------------
	bool InputImpl::IsKeyPressedImpl(AEKey key)
	{
		return m_keyState[static_cast<Size_t>(key)];
	}

	bool InputImpl::IsKeyPressedNoRepeatImpl(AEKey key)
	{
		return IsKeyPressedImpl(key) && !m_keyStateLast[static_cast<Size_t>(key)];
	}

	bool InputImpl::IsMouseButtonPressedImpl(AEMouse button)
	{
		return m_mouseButtonState[static_cast<Size_t>(button)];
	}

	bool InputImpl::IsMouseButtonPressedNoRepeatImpl(AEMouse button)
	{
		return IsMouseButtonPressedImpl(button) && !m_mouseButtonStateLast[static_cast<Size_t>(button)];
	}

	Math::vec2 InputImpl::GetMousePositionImpl()
	{
		return m_mousePosition;
	}

	Math::vec2 InputImpl::GetMouseDeltaImpl()
	{
		return m_mousePosition - m_mousePositionLast;
	}

	Math::vec2 InputImpl::GetMouseScrollImpl()
	{
		return m_mouseScroll;
	}

//--------------------------------------------------------------------------------
// Backend functions
//--------------------------------------------------------------------------------
	InputImpl& InputImpl::Instance()
	{
		static InputImpl instance;
		return instance;
	}

	void InputImpl::OnUpdate()
	{
		for (Size_t i = 0; i < static_cast<Size_t>(AEKey::INVALID); i++)
		{
			m_keyStateLast[i] = m_keyState[i];
		}

		for (Size_t i = 0; i < static_cast<Size_t>(AEMouse::INVALID); i++)
		{
			m_mouseButtonStateLast[i] = m_mouseButtonState[i];
		}

		m_mousePositionLast = m_mousePosition;
		m_mouseScroll = Math::vec2{ 0.0f, 0.0f };
	}

	void InputImpl::SetKeyState(AEKey key, bool state)
	{
		m_keyState[static_cast<Size_t>(key)] = state;
	}

	void InputImpl::SetMouseButtonState(AEMouse button, bool state)
	{
		m_mouseButtonState[static_cast<Size_t>(button)] = state;
	}

	void InputImpl::SetMousePosition(const Math::vec2& position)
	{
		m_mousePosition = position;
	}

	void InputImpl::SetMouseScroll(const Math::vec2& scroll)
	{
		m_mouseScroll = scroll;
	}
}
