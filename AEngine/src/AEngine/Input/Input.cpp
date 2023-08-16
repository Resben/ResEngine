#include "Input.h"
#include "InputBuffer.h"

namespace AEngine
{
	bool Input::IsKeyPressed(AEKey key)
	{
		return InputBuffer::Instance().IsKeyPressed(key);
	}

	bool Input::IsKeyPressedNoRepeat(AEKey key)
	{
		return InputBuffer::Instance().IsKeyPressedNoRepeat(key);
	}

	bool Input::IsMouseButtonPressed(AEMouse button)
	{
		return InputBuffer::Instance().IsMouseButtonPressed(button);
	}

	bool Input::IsMouseButtonPressedNoRepeat(AEMouse button)
	{
		return InputBuffer::Instance().IsMouseButtonPressedNoRepeat(button);
	}

	Math::vec2 Input::GetMousePosition()
	{
		return InputBuffer::Instance().GetMousePosition();
	}

	Math::vec2 Input::GetMouseDelta()
	{
		return InputBuffer::Instance().GetMouseDelta();
	}

	Math::vec2 Input::GetMouseScroll()
	{
		return InputBuffer::Instance().GetMouseScroll();
	}
}
