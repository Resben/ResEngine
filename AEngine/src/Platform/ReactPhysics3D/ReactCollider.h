/**
 * \file   ReactCollider.h
 * \author Lane O'Rafferty (33534304)
 * \author Christien Alden (34119981)
*/
#pragma once
#include "ReactCollisionBody.h"
#include "AEngine/Physics/Collider.h"
#include <reactphysics3d/reactphysics3d.h>

namespace AEngine
{
		/**
		 * \class ReactCollider
		 * \brief Represents a collider using the ReactPhysics3D library.
		 * \details
		 * Does not inherit from AEngine::Collider as this causes issues with diamond inheritance.
		 * Instead this class is used as a factory to generate the rp3d collider objects.
		 * Also to wrap the common functionality of the rp3d collider objects.
		*/
	class ReactCollider
	{
	public:
			/**
			 * \brief Constructs a ReactCollider object with the given collision body and shape.
			 *
			 * \param body The collision body associated with the collider.
			 * \param shape The collision shape of the collider.
			 */
		ReactCollider(rp3d::CollisionBody* body, rp3d::CollisionShape* shape);
		~ReactCollider();
			/**
			 * \brief Sets whether the collider is a trigger or not.
			 *
			 * Overrides the base class function SetIsTrigger.
			 *
			 * \param[in] isTrigger Specifies if the collider is a trigger or not.
			 */
		virtual void SetIsTrigger(bool isTrigger);
			/**
			 * \brief Returns whether the collider is a trigger or not.
			 *
			 * Overrides the base class function GetIsTrigger.
			 *
			 * \return True if the collider is a trigger, false otherwise.
			 */
		virtual bool GetIsTrigger() const;
			/**
			 * \brief Returns the native ReactPhysics3D CollisionShape object.
			 *
			 * \return The native ReactPhysics3D CollisionShape object.
			 */
		rp3d::CollisionShape* GetNativeShape() const;

	private:
		rp3d::CollisionShape* m_shape;   ///< The native ReactPhysics3D CollisionShape object.
		rp3d::Collider* m_collider;      ///< The native ReactPhysics3D Collider object.
		rp3d::CollisionBody* m_body;     ///< The native body that the collider is attached to.
    };


	class ReactBoxCollider : public BoxCollider
	{
	public:
		ReactBoxCollider(rp3d::CollisionBody* body, const Math::vec3& size);
		virtual ~ReactBoxCollider() override = default;
		virtual void SetIsTrigger(bool isTrigger) override;
		virtual bool GetIsTrigger() const override;
		virtual void Resize(const Math::vec3& size) override;
		virtual Math::vec3 GetSize() const override;

	private:
		UniquePtr<ReactCollider> m_collider;
	};
}
