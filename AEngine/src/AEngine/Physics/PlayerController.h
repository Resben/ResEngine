/**
 * \file
 * \author Lane O'Rafferty (33534304)
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
			 * \brief Players current falling speed
			 * This will increase as the player falls
			*/
		float m_fallingSpeed;
			/**
			 * \brief The current velocity of the player in world space
			*/
		Math::vec3 m_currentVelocity;

			/**
			 * \brief The rigidbody of the player
			*/
		RigidBody* m_body;
			/**
			 * \brief The raycaster used to detect the ground
			*/
		Raycaster* m_raycaster;

			/**
			 * \brief Determines the state of the player, grounded or falling
			 * \retval true if player is on ground
			 * \retval false if player is NOT on the ground
			*/
		bool DetectState();
			/**
			 * \brief Projects the player's movement direction to be parallel with a surface
			 * \param[in] direction the movement direction
			 * \param[in] normal the surface normal
			 *
			 * Uses the 'collide and slide' technique for player motion
			*/
		Math::vec3 DirectionFromNormal(const Math::vec3& direction, const Math::vec3& normal);
	};
}



