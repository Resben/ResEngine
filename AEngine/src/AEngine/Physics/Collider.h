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
		enum class Type
		{
			Box
		};

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
			/**
			 * \brief Gets the type of the collider.
			 * \return The type of the collider.
			*/
		virtual Type GetType() const = 0;
		virtual const char* GetName() const = 0;
	};


	class BoxCollider : public Collider
	{
	public:
		virtual void SetIsTrigger(bool isTrigger) = 0;
		virtual bool GetIsTrigger() const = 0;
		virtual void Resize(const Math::vec3& size) = 0;
		virtual Math::vec3 GetSize() const = 0;
		virtual Type GetType() const override { return Type::Box; }
		virtual const char* GetName() const override { return "Box"; }
	};
}
