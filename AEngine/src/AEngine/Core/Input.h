/**
 * @file
 * @author Christien Alden (344119981)
**/
#pragma once
#include "AEngine/Math/Math.h"
#include "KeyCodes.h"

namespace AEngine
{
		/**
		 * @class Input
		 * @brief Allows querying of input directly, instead of through events
		**/
	class Input
	{
	public:
			/**
			 * \brief Checks if a key is currently pressed
			 * \param[in] key The key to check
			 * \retval true The key is currently pressed
			 * \retval false The key is not currently pressed
			*/
		static bool IsKeyPressed(AEKey key);
			/**
			 * \brief Checks if a key is currently pressed and was not pressed in the previous frame
			 * \param[in] key The key to check
			 * \retval true The key is currently pressed and was not pressed in the previous frame
			 * \retval false The key is not currently pressed or was pressed in the previous frame
			*/
		static bool IsKeyPressedNoRepeat(AEKey key);
			/**
			 * \brief Checks if a mouse button is currently pressed
			 * \param[in] button The mouse button to check
			 * \retval true The mouse button is currently pressed
			 * \retval false The mouse button is not currently pressed
			*/
		static bool IsMouseButtonPressed(AEMouse button);
			/**
			 * \brief Checks if a mouse button is currently pressed and was not pressed in the previous frame
			 * \param[in] button The mouse button to check
			 * \retval true The mouse button is currently pressed and was not pressed in the previous frame
			 * \retval false The mouse button is not currently pressed or was pressed in the previous frame
			*/
		static bool IsMouseButtonPressedNoRepeat(AEMouse button);
			/**
			 * \brief Gets the current mouse position
			 * \return The current mouse position
			*/
		static Math::vec2 GetMousePosition();
			/**
			 * \brief Gets the change in mouse position since the last frame
			 * \return The change in mouse position since the last frame
			*/
		static Math::vec2 GetMouseDelta();
			/**
			 * \brief Gets the current mouse scroll
			 * \return The current mouse scroll
			*/
		static Math::vec2 GetMouseScroll();
	};
}
