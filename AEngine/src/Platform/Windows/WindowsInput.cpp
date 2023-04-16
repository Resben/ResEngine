/**
 * @file
 * @author Christien Alden (34119981)
**/
#include "WindowsInput.h"
#include "WindowsKeys.h"

namespace AEngine
{
	GLFWInput::GLFWInput(GLFWwindow* context)
		: InputQuery(), m_context(context)
	{
	}

	bool GLFWInput::IsKeyPressed(AEKey key) const
	{
		return glfwGetKey(m_context, ToGLFWKey(key)) == GLFW_PRESS;
	}

	bool GLFWInput::IsMouseButtonPressed(AEMouse button) const
	{
		return glfwGetMouseButton(m_context, ToGLFWMouse(button)) == GLFW_PRESS;
	}

	Math::vec2 GLFWInput::GetMousePosition() const
	{
		double xpos, ypos;
		glfwGetCursorPos(m_context, &xpos, &ypos);
		return { (float) xpos, (float) ypos };
	}
}
