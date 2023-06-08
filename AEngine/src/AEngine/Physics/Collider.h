/**
 * \file   Collider.h
 * \author Lane O'Rafferty (33534304)
 * \author Christien Alden (34119981)
*/

#pragma once

#include "AEngine/Math/Math.h"

namespace AEngine
{
		/**
		 * \class Collider
		 * \brief Represents a collider in the physics simulation.
		 *
		 * This class provides an interface for managing collider properties.
		 */
	class Collider
	{
	public:
		virtual ~Collider() = default;
			/**
			 * \brief Sets whether the collider is a trigger.
			 *
			 * \param[in] isTrigger True to set the collider as a trigger, false otherwise.
			 */
		virtual void SetIsTrigger(bool isTrigger) = 0;
			/**
			 * \brief Checks if the collider is a trigger.
			 *
			 * \return True if the collider is a trigger, false otherwise.
			 */
		virtual bool GetIsTrigger() const = 0;
	};
}
