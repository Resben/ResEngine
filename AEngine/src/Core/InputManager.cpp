/**
 * @file
 * @author Ben Hawkins (34112619)
 * @author Christien Alden (34119981)
 * @brief Stores key/mouse states in arrays, functions to prevent repeating keys
**/
#include "InputManager.h"

namespace AEngine
{
	#define UP false
	#define DOWN true

	//-------------------------------------------------
	//		INPUT MANAGER
	//-------------------------------------------------

	bool InputManager::s_keyState[1024];
	bool InputManager::s_keyLast[1024];
	bool InputManager::s_mouseState[8];
	bool InputManager::s_mouseLast[8];
	Math::vec2 InputManager::s_mousePos;
	Math::vec2 InputManager::s_lastMousePos;

		// Called before glfwPollEvents to store previous keys
	void InputManager::OnUpdate()
	{
		// save key state
		for (int i = 0; i < 1024; ++i)
		{
			s_keyLast[i] = s_keyState[i];
		}

		// save mouse button state
		for (int y = 0; y < 8; ++y)
		{
			s_mouseLast[y] = s_mouseState[y];
		}

		// save cursor state
		s_lastMousePos = s_mousePos;
	}

	void InputManager::Init()
	{
		s_mousePos = {0.0f, 0.0f};

		for (int i = 0; i < 1024; ++i)
		{
			s_keyState[i] = s_keyLast[i] = UP;
		}

		for (int y = 0; y < 8; ++y)
		{
			s_mouseState[y] = s_mouseLast[y] = UP;
		}
	}

	int InputManager::ToAE(int code)
	{
		assert(false); // not implemented
		return 1;
	}

	//-------------------------------------------------
	//		CALLBACKS
	//-------------------------------------------------

	void InputManager::MouseCallback(GLFWwindow* callbackContext, int button, int action, int mods)
	{
		//int button = ToAE(button);

		if (action == GLFW_PRESS)
			s_mouseState[button] = DOWN;
		else if (action == GLFW_RELEASE)
			s_mouseState[button] = UP;
	}

	void InputManager::MouseMoveCallback(GLFWwindow* callbackContext, double xPos, double yPos)
	{
		s_mousePos.x = static_cast<float>(xPos);
		s_mousePos.y = static_cast<float>(yPos);
	}

	void InputManager::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		//int key = ToAE(key);

		if (action == GLFW_PRESS)
			s_keyState[key] = DOWN;
		else if (action == GLFW_RELEASE)
			s_keyState[key] = UP;
}

	//-------------------------------------------------
	//		KEY MANAGER
	//-------------------------------------------------

	bool InputManager::IsKeyPressed(int keycode)
	{
		return s_keyState[keycode] == DOWN;
	}

	bool InputManager::IsKeyNoRepeat(int keycode)
	{
		return (s_keyState[keycode] == DOWN) && (s_keyLast[keycode] != DOWN);
	}

	bool InputManager::IsKeyUpNoRepeat(int keycode)
	{
		return (s_keyState[keycode] == UP) && (s_keyLast[keycode] != UP);
	}

	bool InputManager::IsKeyUp(int keycode)
	{
		return s_keyState[keycode] == UP;
	}

	//-------------------------------------------------
	//		MOUSE MANAGER
	//-------------------------------------------------

	bool InputManager::IsMousePressed(int button)
	{
		return s_mouseState[button] == DOWN;
	}

	bool InputManager::IsMouseNoRepeat(int button)
	{
		return (s_mouseState[button] == DOWN) && (s_mouseLast[button] != DOWN);
	}

	bool InputManager::IsMouseUpNoRepeat(int button)
	{
		return (s_mouseState[button] == UP) && (s_mouseLast[button] != UP);
	}

	bool InputManager::IsMouseUp(int button)
	{
		return s_mouseState[button] == UP;
	}

	Math::vec2 InputManager::GetMousePos()
	{
		return s_mousePos;
	}

	Math::vec2 InputManager::GetLastMousePos()
	{
		return s_lastMousePos;
	}

	Math::vec2 InputManager::GetMouseDelta()
	{
		return s_mousePos - s_lastMousePos;
	}
}
