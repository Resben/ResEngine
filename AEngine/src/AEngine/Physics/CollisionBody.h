/**
 * \file   CollisionBody.h
 * \author Lane O'Rafferty (33534304)
 * \author Christien Alden (34119981)
*/

#pragma once
#include "AEngine/Core/Types.h"
#include "AEngine/Math/Math.h"
#include "Collider.h"

namespace AEngine
{
	class CollisionBody
	{
	public:
		virtual ~CollisionBody() {
			m_collider = nullptr;
		}

			/**
			 * \brief Sets the transform (position and orientation) of the collision body.
			 *
			 * \param[in] position The position vector.
			 * \param[in] orientation The orientation quaternion.
			 */
		virtual void SetTransform(const Math::vec3& position, const Math::quat& orientation) = 0;
			/**
			 * \brief Gets the transform (position and orientation) of the collision body.
			 *
			 * \param[out] position The output parameter to store the position vector.
			 * \param[out] orientation The output parameter to store the orientation quaternion.
			 */
		virtual void GetTransform(Math::vec3& position, Math::quat& orientation) const = 0;
			/**
			 * \brief Adds a box collider to the collision body.
			 *
			 * \param[in] size The size of the box collider.
			 * \return Pointer to the created collider.
			 */
		virtual Collider* AddBoxCollider(const Math::vec3& size) = 0;
			/**
			 * \brief Adds a sphere collider to the collision body.
			 *
			 * \param[in] radius The radius of the sphere collider.
			 * \return Pointer to the created collider.
			 */
		virtual Collider* AddSphereCollider(float radius) = 0;
			/**
			 * \brief Adds a capsule collider to the collision body.
			 *
			 * \param[in] radius The radius of the capsule collider.
			 * \param[in] height The height of the capsule collider.
			 * \return Pointer to the created collider.
			 */
		virtual Collider* AddCapsuleCollider(float radius, float height) = 0;
			/**
			 * \brief Adds a height map collider to the collision body.
			 *
			 * \param[in] sideLength The length of the square side of the height map.
			 * \param[in] minHeight The minimum height of the height map.
			 * \param[in] maxHeight The maximum height of the height map.
			 * \param[in] data The pointer to the height map data.
			 * \param[in] scale The scale vector of the height map.
			 * \return Pointer to the created collider.
			 */
		virtual Collider* AddHeightMapCollider(int sideLength, float minHeight, float maxHeight, const float* data, const Math::vec3& scale) = 0;

		Collider* GetCollider() { return m_collider.get(); }

			/**
			 * \brief Removes a collider from the collision body.
			 */
		virtual void RemoveCollider() { m_collider = nullptr; };

			/**
			 * \brief Gets the interpolated transform (position and orientation) of the collision body.
			 *
			 * \param[out] position The output parameter to store the interpolated position vector.
			 * \param[out] orientation The output parameter to store the interpolated orientation quaternion.
			 */
		virtual void GetInterpolatedTransform(Math::vec3& position, Math::quat& orientation) = 0;

	protected:
			/**
			 * \brief Holds the collider associated with the collision body.
			*/
		UniquePtr<Collider> m_collider;
	};

	class RigidBody : public CollisionBody
	{
	public:
		enum class Type {
			STATIC,      ///< Static bodies are not affected by forces like gravity, and they cannot move.
			KINEMATIC,   ///< Kinematic bodies are special types of static bodies that can be moved by the user, but are not affected by forces like gravity.
			DYNAMIC      ///< Dynamic bodies are fully simulated. They can be moved manually by the user, but normally they are moved according to forces.
		};

	public:
		virtual ~RigidBody() = default;
			/**
			 * \brief Sets whether the rigid body has gravity.
			 *
			 * \param[in] hasGravity True to enable gravity, false otherwise.
			 */
		virtual void SetHasGravity(bool hasGravity) = 0;
			/**
			 * \brief Checks if the rigid body has gravity.
			 *
			 * \return True if the rigid body has gravity, false otherwise.
			 */
		virtual bool GetHasGravity() const = 0;
			/**
			 * \brief Sets the mass of the rigid body.
			 *
			 * \param[in] massKg The mass of the rigid body in kilograms.
			 */
		virtual void SetMass(float massKg) = 0;
			/**
			 * \brief Gets the mass of the rigid body.
			 *
			 * \return The mass of the rigid body in kilograms.
			 */
		virtual float GetMass() const = 0;
			/**
			 * \brief Sets the velocity of the rigid body.
			 *
			 * \param[in] velocity The velocity vector of the rigid body.
			 */
		virtual void SetVelocity(const Math::vec3& velocity) = 0;
			/**
			 * \brief Gets the velocity of the rigid body.
			 *
			 * \return The velocity vector of the rigid body.
			 */
		virtual const Math::vec3 GetVelocity() const = 0;
			/**
			 * \brief Sets the drag factor of the rigid body.
			 *
			 * \param[in] factor The drag factor of the rigid body.
			 */
		virtual void SetDrag(float factor) = 0;
			/**
			 * \brief Sets the type of the rigid body.
			 *
			 * \param[in] type The type of the rigid body.
			 */
		virtual void SetType(Type type) = 0;
	};
}
