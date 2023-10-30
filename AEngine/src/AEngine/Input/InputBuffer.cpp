#include "InputBuffer.h"
#include "AEngine/Core/Application.h"

namespace AEngine
{
//--------------------------------------------------------------------------------
// Polling
//--------------------------------------------------------------------------------
	AEInputState InputBuffer::GetKey(AEKey key) const
	{
		if (key == AEKey::INVALID)
		{
			return AEInputState::Unknown;
		}

		// return the key state
		return m_keyState[static_cast<Size_t>(key)];
	}

	bool InputBuffer::IsKeyDown(AEKey key) const
	{
		if (key == AEKey::INVALID)
		{
			return false;
		}

		AEInputState state = GetKey(key);
		return {
			// check if the key is pressed or repeated
			state == AEInputState::Pressed ||
			state == AEInputState::Repeated
		};
	}

	bool InputBuffer::IsKeyUp(AEKey key) const
	{
		if (key == AEKey::INVALID)
		{
			return false;
		}

		AEInputState state = GetKey(key);
		return {
			state == AEInputState::Released
		};
	}

	AEInputState InputBuffer::GetMouseButton(AEMouse button) const
	{
		if (button == AEMouse::INVALID)
		{
			return AEInputState::Unknown;
		}

		// return the mouse button state
		return m_mouseState[static_cast<Size_t>(button)];
	}

	bool InputBuffer::IsMouseButtonDown(AEMouse button) const
	{
		if (button == AEMouse::INVALID)
		{
			return false;
		}

		AEInputState state = GetMouseButton(button);
		return {
			state == AEInputState::Pressed ||
			state == AEInputState::Repeated
		};
	}

	bool InputBuffer::IsMouseButtonUp(AEMouse button) const
	{
		if (button == AEMouse::INVALID)
		{
			return false;
		}

		AEInputState state = GetMouseButton(button);
		return {
			state == AEInputState::Released
		};
	}

	Math::vec2 InputBuffer::GetMousePosition() const
	{
		return m_mousePosition;
	}

	Math::vec2 InputBuffer::GetMouseDelta() const
	{
		return m_mouseDelta;
	}

	Math::vec2 InputBuffer::GetMouseScroll() const
	{
		return m_mouseScroll;
	}

//--------------------------------------------------------------------------------
// Updating
//--------------------------------------------------------------------------------
	void InputBuffer::Reset()
	{
		// reset the key states
		m_keyState.fill(AEInputState::Released);
		m_mouseState.fill(AEInputState::Released);

		// reset the mouse state
		m_mousePosition = Math::vec2(0.0f);
		m_mouseDelta = Math::vec2(0.0f);
		m_mouseScroll = Math::vec2(0.0f);
	}

	void InputBuffer::SetKeyState(AEKey key, AEInputState state)
	{
		if (key == AEKey::INVALID)
		{
			return;
		}

		m_keyState[static_cast<Size_t>(key)] = state;
	}

	void InputBuffer::SetMouseButtonState(AEMouse button, AEInputState state)
	{
		if (button == AEMouse::INVALID)
		{
			return;
		}

		m_mouseState[static_cast<Size_t>(button)] = state;
	}

	void InputBuffer::SetMouseState(const Math::vec2& position, const Math::vec2& delta)
	{
		m_mousePosition = position;
		m_mouseDelta = delta;
	}

	void InputBuffer::SetMouseScroll(const Math::vec2& scroll)
	{
		m_mouseScroll = scroll;
	}

	InputBuffer &InputBuffer::operator=(const InputBuffer &rhs)
	{
		m_keyState = rhs.m_keyState;
		m_mouseState = rhs.m_mouseState;
		m_mousePosition = rhs.m_mousePosition;
		m_mouseDelta = rhs.m_mouseDelta;
		m_mouseScroll = rhs.m_mouseScroll;
		return *this;
	}
}
