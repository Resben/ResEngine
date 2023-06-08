/**
 * \file   ReactCollider.h
 * \author Lane O'Rafferty (33534304)
 * \author Christien Alden (34119981)
*/

#pragma once

#include <reactphysics3d/reactphysics3d.h>
#include <AEngine/Physics/Collider.h>
#include "ReactCollisionBody.h"

namespace AEngine
{
		/**
		 * \class ReactCollider
		 * \brief Represents a collider using the ReactPhysics3D library.
		 *
		 * This class inherits from Collider.
		 */
	class ReactCollider : public Collider
	{
	public:
			/**
			 * \brief Constructs a ReactCollider object with the given collision body and shape.
			 *
			 * \param body The collision body associated with the collider.
			 * \param shape The collision shape of the collider.
			 */
		ReactCollider(rp3d::CollisionBody* body, rp3d::CollisionShape* shape);
			/**
			 * \brief Default destructor.
			 */
		virtual ~ReactCollider() = default;
			/**
			 * \brief Sets whether the collider is a trigger or not.
			 *
			 * Overrides the base class function SetIsTrigger.
			 *
			 * \param[in] isTrigger Specifies if the collider is a trigger or not.
			 */
		virtual void SetIsTrigger(bool isTrigger) override;
			/**
			 * \brief Returns whether the collider is a trigger or not.
			 *
			 * Overrides the base class function GetIsTrigger.
			 *
			 * \return True if the collider is a trigger, false otherwise.
			 */
		virtual bool GetIsTrigger() const override;
			/**
			 * \brief Returns the native collider object.
			 *
			 * \return A pointer to the native ReactPhysics3D Collider object.
			 */
		rp3d::Collider* GetNative();

	private:
		rp3d::Collider* m_collider;		///< The native ReactPhysics3D Collider object.
    };
}
