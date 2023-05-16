#include "Input.h"
#include "InputImpl.h"

namespace AEngine
{
	bool Input::IsKeyPressed(AEKey key)
	{
		return InputImpl::Instance().IsKeyPressedImpl(key);
	}

	bool Input::IsKeyPressedNoRepeat(AEKey key)
	{
		return InputImpl::Instance().IsKeyPressedNoRepeatImpl(key);
	}

	bool Input::IsMouseButtonPressed(AEMouse button)
	{
		return InputImpl::Instance().IsMouseButtonPressedImpl(button);
	}

	bool Input::IsMouseButtonPressedNoRepeat(AEMouse button)
	{
		return InputImpl::Instance().IsMouseButtonPressedNoRepeatImpl(button);
	}

	Math::vec2 Input::GetMousePosition()
	{
		return InputImpl::Instance().GetMousePositionImpl();
	}

	Math::vec2 Input::GetMouseDelta()
	{
		return InputImpl::Instance().GetMouseDeltaImpl();
	}

	Math::vec2 Input::GetMouseScroll()
	{
		return InputImpl::Instance().GetMouseScrollImpl();
	}
}
