/**
 * \file PlayerController.h
 * \author Lane O'Rafferty (33534304)
 * \author Christien Alden (34119981)
*/

#pragma once
#include "AEngine/Math/Math.h"
#include "CollisionBody.h"
#include "Physics.h"
#include "Raycaster.h"

namespace AEngine
{
	/**
		* \class PlayerController
		* \brief PlayerController Class to be used by the scripting system
		* \author Lane O'Rafferty (33534304)
	**/
	class PlayerController
	{
	public:
			/**
			 * \struct Properties
			 * \brief The properties of the player
			*/
		struct Properties
		{
				/**
				 * \brief The radius of the capsule
				*/
			float radius = 0.50f;
				/**
				 * \brief The height of the capsule
				*/
			float height = 1.80f;
				/**
				 * \brief The factor to move the player by
				*/
			float moveFactor = 10.5f;
				/**
				 * \brief The drag to apply to the player when moving
				*/
			float moveDrag = 7.0f;
				/**
				 * \brief The drag to apply to the player when falling
				*/
			float fallDrag = 0.0f;
		};

	public:
			/**
			 * \param[in] world The physics world
			 * \param[in] startPosition The starting position of the player
			 * \param[in] prop The properties of the player
			*/
		PlayerController(
			PhysicsWorld* world,
			const Math::vec3& startPosition,
			const Properties& prop
		);

		~PlayerController() = default;

			/**
			 * \brief Moves the player in the physics world
			 * \param[in] direction the direction to move in world space
			 *
			 * Used for player movement
			*/
		void ApplyForce(Math::vec3& direction);
			/**
			 * \brief Provides a falling motion for the player
			 * \param [dt] the frametime passed
			 *
			 * To be used in the on update loop
			**/
		void OnUpdate(float dt);
			/**
			 * \brief Returns the players transform
			 * \note This should be called after OnUpdate()
			*/
		Math::vec3 GetTransform() const;

		void SetSpeed(float speed);

	private:
			/**
			 * \brief The properties of the player
			*/
		Properties m_properties;
			/**
			 * \brief Height of the player
			*/
		float m_capsuleHeight;
			/**
			 * \brief Signifies that the player is in a grounded state
			*/
		bool m_inGroundedState;
			/**
			 * \brief Signifies that the player is in a falling state
			*/
		bool m_inFallingState;
			/**
			 * \brief Signifies that the player hit a surface in the direction they had moved in
			*/
		bool m_hitWall;
			/**
			 * \brief Players current falling speed
			 * This will increase as the player falls
			*/
		float m_fallingSpeed;
			/**
			 * \brief The current direction the player is moving in world space
			*/
		Math::vec3 m_currentDirection;
			/**
			 * \brief The current direction the player is moving in world space limited to unit length
			*/
		Math::vec3 m_unitDirection;
			/**
			 * \brief The current velocity of the player in world space
			*/
		Math::vec3 m_currentVelocity;
			/**
			 * \brief The rigidbody of the player
			*/
		SharedPtr<RigidBody> m_body;
			/**
			 * \brief The raycaster used to detect the ground collisions
			*/
		Raycaster* m_groundRay;
			/**
			 * \brief The raycaster used to detect wall collisions
			*/
		Raycaster* m_forwardRay;

		float m_forwardRayLength;
		float m_groundRayLength;

			/**
			 * \brief Determines the state of the player, grounded or falling
			 * \retval true if collision occurred
			 * \retval false if no collision occurred
			*/
		bool DetectGround();
			/**
			 * \brief Determines if the player has run into a wall
			 * \retval true if collision occurred
			 * \retval false if no collision occurred
			*/
		bool DetectWall();
			/**
			 * \brief Projects the player's movement direction to be parallel with a surface
			 * \param[in] normal the surface normal
			 *
			 * Uses the 'collide and slide' technique for player motion
			*/
		void MoveAlongNormal(const Math::vec3& normal);
	};
}
