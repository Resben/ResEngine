/**
 * @file
 * @author Christien Alden (34119981)
**/
#pragma once
#include <GLFW/glfw3.h>
#include "AEngine/Core/Input.h"

namespace AEngine
{
	class GLFWInput : public InputQuery
	{
	public:
		GLFWInput(GLFWwindow* context);
		bool IsKeyPressed(AEKey key) const override;
		bool IsMouseButtonPressed(AEMouse button) const override;
		Math::vec2 GetMousePosition() const override;
		Math::vec2 GetMouseDelta() const override;

	private:
		GLFWwindow* m_context;
	};
}
