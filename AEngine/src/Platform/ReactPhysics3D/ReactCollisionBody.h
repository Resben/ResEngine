/**
 * \file   ReactCollisionBody.h
 * \author Lane O'Rafferty (33534304)
 * \author Christien Alden (34119981)
*/

#pragma once
#include <reactphysics3d/reactphysics3d.h>
#include "AEngine/Math/Math.h"
#include "AEngine/Physics/Collider.h"
#include "AEngine/Physics/CollisionBody.h"
#include "Platform/ReactPhysics3D/ReactPhysics.h"

namespace AEngine
{
		/**
		 * \class ReactCollisionBody
		 * \brief Represents a collision body using the ReactPhysics3D library.
		 *
		 * This class inherits from CollisionBody.
		 */
	class ReactCollisionBody : public CollisionBody
	{
	public:
			/**
			 * \brief Constructs a ReactCollisionBody object with the given parameters.
			 * \param[in] world The ReactPhysicsWorld associated with the collision body.
			 * \param[in] position The initial position of the collision body.
			 * \param[in] orientation The initial orientation of the collision body.
			 */
		ReactCollisionBody(ReactPhysicsWorld* world, const Math::vec3& position, const Math::quat& orientation);
		virtual ~ReactCollisionBody();

			/**
			 * \copydoc CollisionBody::SetTransform
			 */
		virtual void SetTransform(const Math::vec3&, const Math::quat&) override;
			/**
			 * \copydoc CollisionBody::GetTransform
			 */
		virtual void GetTransform(Math::vec3&, Math::quat&) const override;
			/**
			 * \copydoc CollisionBody::AddBoxCollider
			 */
		virtual UniquePtr<Collider> AddBoxCollider(const Math::vec3& size, const Math::vec3& offset = Math::vec3{0.0f}, const Math::quat& orientation = Math::quat{Math::vec3{ 0.0f, 0.0f, 0.0f }}) override;
			/**
			 * \copydoc CollisionBody::AddCapsuleCollider
			*/
		virtual UniquePtr<Collider> AddCapsuleCollider(float radius, float height, const Math::vec3& offset = Math::vec3{0.0f}, const Math::quat& orientation = Math::quat{Math::vec3{ 0.0f, 0.0f, 0.0f }}) override;
			/**
			 * \copydoc CollisionBody::AddSphereCollider
			*/
		virtual UniquePtr<Collider> AddSphereCollider(float radius, const Math::vec3& offset = Math::vec3{0.0f}, const Math::quat& orientation = Math::quat{Math::vec3{ 0.0f, 0.0f, 0.0f }}) override;
			/**
			 * \copydoc CollisionBody::GetCollider
			*/
		virtual UniquePtr<Collider> GetCollider() override;
			/**
			 * \copydoc CollisionBody::RemoveCollider
			*/
		virtual void RemoveCollider() override;

			/**
			 * \copydoc CollisionBody::GetCollider
			 */
		virtual void GetInterpolatedTransform(Math::vec3& position, Math::quat& orientation) override;

			/**
			 * \brief Returns the native collision body object.
			 * \return A pointer to the native ReactPhysics3D CollisionBody object.
			 */
		rp3d::CollisionBody* GetNative() const;

	protected:
		rp3d::CollisionBody* m_body;       ///< The native ReactPhysics3D CollisionBody object.
		ReactPhysicsWorld* m_world;        ///< The ReactPhysicsWorld associated with the collision body.
		rp3d::Transform m_lastTransform;   ///< The last transform of the collision body.
	};

		/**
		 * \class ReactRigidBody
		 * \brief Represents a rigid body using the ReactPhysics3D library.
		 *
		 * This class inherits from RigidBody.
		 */
	class ReactRigidBody : public RigidBody
	{
	public:
			/**
			 * \brief Constructs a ReactRigidBody object with the given parameters.
			 * \param world The ReactPhysicsWorld associated with the rigid body.
			 * \param position The initial position of the rigid body.
			 * \param orientation The initial orientation of the rigid body.
			 */
		ReactRigidBody(ReactPhysicsWorld* world, const Math::vec3& position, const Math::quat& orientation);
		virtual ~ReactRigidBody() = default;

			/**
			 * \copydoc RigidBody::SetType
			 */
		virtual void SetType(Type type) override;
			/**
			 * \copydoc RigidBody::GetType
			*/
		virtual Type GetType() const override;

			/**
			 * \copydoc RigidBody::SetMass
			 */
		virtual void SetMass(float massKg) override;
			/**
			 * \copydoc RigidBody::GetMass
			 */
		virtual float GetMass() const override;
			/**
			 * \copydoc RigidBody::GetInverseMass
			*/
		virtual float GetInverseMass() const override;
			/**
			 * \copydoc RigidBody::SetRestitution
			 */
		virtual void SetRestitution(float restitution) override;
			/**
			 * \copydoc RigidBody::GetRestitution
			 */
		virtual float GetRestitution() const override;
			/**
			 * \copydoc RigidBody::GetLocalInertiaTensor
			 */
		virtual Math::mat3 GetLocalInertiaTensor() const override;
			/**
			 * \copydoc RigidBody::GetWorldInertiaTensor
			*/
		virtual Math::mat3 GetWorldInertiaTensor() const override;
			/**
			 * \copydoc RigidBody::GetLocalInverseInertiaTensor
			*/
		virtual Math::mat3 GetLocalInverseInertiaTensor() const override;
			/**
			 * \copydoc RigidBody::GetWorldInverseInertiaTensor
			*/
		virtual Math::mat3 GetWorldInverseInertiaTensor() const override;
			/**
			 * \copydoc RigidBody::SetCentreOfMass
			*/
		virtual void SetCentreOfMass(const Math::vec3& centreOfMass) override;
			/**
			 * \copydoc RigidBody::GetCentreOfMass
			*/
		virtual Math::vec3 GetCentreOfMass() const override;
			/**
			 * \copydoc RigidBody::GetCentreOfMassWorldSpace
			*/
		virtual Math::vec3 GetCentreOfMassWorldSpace() const override;

			/**
			 * \copydoc RigidBody::SetHasGravity
			 */
		virtual void SetHasGravity(bool hasGravity) override;
			/**
			 * \copydoc RigidBody::GetHasGravity
			 */
		virtual bool GetHasGravity() const override;
			/**
			 * \copydoc RigidBody::SetLinearDamping
			*/
		virtual void SetLinearDamping(float damping) override;
			/**
			 * \copydoc RigidBody::GetLinearDamping
			*/
		virtual float GetLinearDamping() const override;
			/**
			 * \copydoc RigidBody::SetAngularDamping
			*/
		virtual void SetAngularDamping(float damping) override;
			/**
			 * \copydoc RigidBody::GetAngularDamping
			*/
		virtual float GetAngularDamping() const override;

			/**
			 * \copydoc RigidBody::SetLinearMomentum
			*/
		virtual void SetLinearMomentum(const Math::vec3& momentum) override;
			/**
			 * \copydoc RigidBody::GetLinearMomentum
			*/
		virtual const Math::vec3 GetLinearMomentum() const override;
			/**
			 * \copydoc RigidBody::SetAngularMomentum
			*/
		virtual void SetAngularMomentum(const Math::vec3& momentum) override;
			/**
			 * \copydoc RigidBody::GetAngularMomentum
			*/
		virtual const Math::vec3 GetAngularMomentum() const override;

			/**
			 * \copydoc RigidBody::SetLinearVelocity
			 */
		virtual void SetLinearVelocity(const Math::vec3& velocity) override;
			/**
			 * \copydoc RigidBody::GetLinearVelocity
			 */
		virtual const Math::vec3 GetLinearVelocity() const override;
			/**
			 * \copydoc RigidBody::SetAngularVelocity
			*/
		virtual void SetAngularVelocity(const Math::vec3& velocity) override;
			/**
			 * \copydoc RigidBody::GetAngularVelocity
			*/
		virtual const Math::vec3 GetAngularVelocity() const override;

		//--------------------------------------------------------------------------------
		// From CollisionBody
		//--------------------------------------------------------------------------------
			/**
			 * \copydoc ReactCollisionBody::SetTransform
			 */
		virtual void SetTransform(const Math::vec3& position, const Math::quat& orientation) override;
			/**
			 * \copydoc ReactCollisionBody::GetTransform
			 */
		virtual void GetTransform(Math::vec3& position, Math::quat& orientation) const override;
			/**
			 * \copydoc ReactCollisionBody::AddBoxCollider
			 */
		virtual UniquePtr<Collider> AddBoxCollider(const Math::vec3& size, const Math::vec3& offset = Math::vec3{0.0f}, const Math::quat& orientation = Math::quat{Math::vec3{ 0.0f, 0.0f, 0.0f }}) override;
			/**
			 * \copydoc ReactCollisionBody::AddCapsuleCollider
			*/
		virtual UniquePtr<Collider> AddCapsuleCollider(float radius, float height, const Math::vec3& offset = Math::vec3{0.0f}, const Math::quat& orientation = Math::quat{Math::vec3{ 0.0f, 0.0f, 0.0f }}) override;
			/**
			 * \copydoc ReactCollisionBody::AddSphereCollider
			*/
		virtual UniquePtr<Collider> AddSphereCollider(float radius, const Math::vec3& offset = Math::vec3{0.0f}, const Math::quat& orientation = Math::quat{Math::vec3{ 0.0f, 0.0f, 0.0f }}) override;
			/**
			 * \copydoc ReactCollisionBody::GetCollider
			*/
		virtual UniquePtr<Collider> GetCollider() override;
			/**
			 * \copydoc ReactCollisionBody::RemoveCollider
			*/
		virtual void RemoveCollider() override;

			/**
			 * \copydoc ReactCollisionBody::GetCollider
			 */
		virtual void GetInterpolatedTransform(Math::vec3& position, Math::quat& orientation) override;

	private:
		UniquePtr<ReactCollisionBody> m_body;                ///< The ReactCollisionBody associated with the rigid body.

		// general properties
		RigidBody::Type m_type{ RigidBody::Type::Dynamic};   ///< The type of the rigid body.
		bool m_hasGravity{ false };                          ///< Specifies if the rigid body has gravity.

		// mass properties
		float m_mass{ 1.0f };                                ///< The mass of the rigid body in kilograms.
		Math::vec3 m_centreOfMass{ 0.0f };                   ///< The centre of mass of the rigid body in local space.
		Math::mat3 m_inertiaTensor{ 0.0f };                  ///< The inertia tensor of the rigid body, in kg/m^2.
		float m_inverseMass{ 1.0f };                         ///< The mass of the of the rigid body.
		Math::mat3 m_inverseInertiaTensor{ 0.0f };           ///< The inverse inertia tensor of the rigid body.

		// velocities
		Math::vec3 m_linearMomentum{ 0.0f };                 ///< The linear momentum of the rigid body world space kg/m/s.
		Math::vec3 m_angularMomentum{ 0.0f };                ///< The angular momentum of the rigid body in kg/m^2/s.

		// material properties
		float m_restitution{ 0.6f };                         ///< The coefficient of restitution of the rigid body, between 0.0f and 1.0f incl.

		// damping properties
		float m_linearDamping{ 0.0f };                       ///< The linear damping factor of the rigid body, between 0.0f and 1.0f incl.
		float m_angularDamping{ 0.0f };                      ///< The angular damping factor of the rigid body, between 0.0f and 1.0f incl.

			/**
			 * \brief Calculates the inertia tensor of the rigid body.
			 * \note This function is called automatically when the mass of the rigid body is set.
			*/
		void CalculateInertiaTensor();
	};
}
