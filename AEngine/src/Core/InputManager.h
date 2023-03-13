/**
 *	@file
 *	@brief Stores key/mouse states in arrays, functions to prevent repeating keys
 *  @author Ben Hawkins (34112619)
 *  @author Christien Alden (34119981)
**/
#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace AEngine
{
	/**
	 * @class InputManager
	 * @brief Input callbacks and stores key/mouse states
	**/
	class InputManager
	{
	public:
		InputManager() = delete;

		/**
		 * @brief Returns if key is pressed or not (repeats)
		 * @param[in] AEngine keycode
		 * @return bool
		**/
		static bool IsKeyPressed(int keycode);

		/**
		 * @brief Returns if key is pressed or not (does not repeat if key is still pressed)
		 * @param[in] AEngine keycode
		 * @return bool
		**/
		static bool IsKeyNoRepeat(int keycode);

		/**
		 * @brief Returns if key is up or not (does not repeat if key is still pressed)
		 * @param[in] AEngine keycode
		 * @return bool
		**/
		static bool IsKeyUpNoRepeat(int keycode);

		/**
		 * @brief Returns if key is up or not (repeats)
		 * @param[in] AEngine keycode
		 * @return bool
		**/
		static bool IsKeyUp(int keycode);


		/**
		* @brief Returns if mouse is pressed or not (repeats)
		* @param[in] AEngine keycode
		* @return bool
		**/
		static bool IsMousePressed(int button);

		/**
		* @brief Returns if mouse is pressed or not (does not repeat if mouse is still pressed)
		* @param[in] AEngine keycode
		* @return bool
		**/
		static bool IsMouseNoRepeat(int button);

		/**
		* @brief Returns if mouse is up or not (does not repeat if mouse is still pressed)
		* @param[in] AEngine keycode
		* @return bool
		**/
		static bool IsMouseUpNoRepeat(int button);

		/**
		* @brief Returns if mouse is up or not (repeats)
		* @param[in] AEngine keycode
		* @return bool
		**/
		static bool IsMouseUp(int button);

		/**
		* @brief Returns a mouse position
		* @return pair of floats
		**/
		static glm::vec2 GetMousePos();

		/**
		* @brief Returns the previous mouse position
		* @return pair of floats
		**/
		static glm::vec2 GetLastMousePos();

		/**
		 * @brief Returns the mouse delta position between ticks
		 * @return pair of floats
		**/
		static glm::vec2 GetMouseDelta();

		/**
		* @brief GLFW key callback function, stores key states
		* @param[in] window: GLFWwindow context
		* @param[in] key: key interacted
		* @param[in] scancode: unique key identifier
		* @param[in] action:  key state
		* @param[in] mods: key modifier
		* @return void
		**/
		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

		/**
		* @brief GLFW mouse callback function, stores mouse button states
		* @param[in] window: GLFWwindow context
		* @param[in] button: mouse button interacted
		* @param[in] action: mouse state
		* @param[in] mods: mouse modifier
		* @return void
		**/
		static void MouseCallback(GLFWwindow* window, int button, int action, int mods);

		/**
		* @brief GLFW mouse move callback function, stores mouse position
		* @param[in] window: GLFWwindow context
		* @param[in] xPos: Mouse x position
		* @param[in] yPos: Mouse y position
		* @return void
		**/
		static void MouseMoveCallback(GLFWwindow* window, double xPos, double yPos);

		/**
		* @brief Initilises values
		* @return void
		**/
		static void Init();

		/**
		* @brief Sets last states of keys, called before glfwPollEvents
		* @return void
		**/
		static void OnUpdate();

		/**
		* @brief Converts GLFW keycodes to AEngine keycodes
		* @return void
		**/
		static int ToAE(int code);

	private:
		static bool s_keyState[1024], s_keyLast[1024], s_mouseState[8], s_mouseLast[8];		/// Arrays for key/mouse states
		static glm::vec2 s_mousePos, s_lastMousePos;										/// Mouse position
	};
}