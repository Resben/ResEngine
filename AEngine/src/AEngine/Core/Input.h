/**
 * @file
 * @author Christien Alden (344119981)
**/
#pragma once
#include "KeyCodes.h"
#include "AEngine/Math/Math.hpp"

namespace AEngine
{
		/**
		 * @class InputQuery
		 * @brief Allows querying of the underlying input engine directly
		**/
	class InputQuery
	{
	public:
			/**
			 * Returns whether a key is pressed
			 * @param[in] key to query
			 * @retval True if pressed
			 * @retval False if not pressed
			**/
		virtual bool IsKeyPressed(AEKey key) const = 0;
			/**
			 * Returns whether a mouse button is pressed
			 * @param[in] button to query
			 * @retval True if pressed
			 * @retval False if not pressed
			**/
		virtual bool IsMouseButtonPressed(AEMouse button) const = 0;
			/**
			 * Returns the current cursor position
			 * @returns Vec2 for x/y position
			**/
		virtual Math::vec2 GetMousePosition() const = 0;

	protected:
		InputQuery() = default;
	};
}
