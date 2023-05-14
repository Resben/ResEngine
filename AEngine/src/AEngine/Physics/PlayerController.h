#pragma once

#include "AEngine/Math/Math.h"
#include "Physics.h"
#include "CollisionBody.h"
#include "Raycaster.h"

namespace AEngine
{
	/**
		* @class PlayerController
		* @brief PlayerController Class to be used by the scripting system
		* @author Lane O'Rafferty (33534304)
	**/
	class PlayerController
	{
	public:
		struct Prop
		{
			float MoveFactor = 10.5f;
			float MoveDrag = 7.f;
			float FallDrag = 0.f;
			float Sensitivity = 0.1f;
			Math::vec3 WorldUp = { 0, 1.f, 0 };
		};

		PlayerController(PhysicsWorld& world, const Math::vec3& startPosition,
			float radius, float height, const Prop& prop);

		~PlayerController() = default;

		/**
			* @brief Returns the players transform
			* @param[position] the player's position
			* @param[orientation] the player's orientation
			*
			* Used to provide the players transform attribute
		**/
		const void GetTransform(Math::vec3& position, Math::quat& orientation) const;

		/**
			* @brief Returns the direction player is facing
			* @return the direction player is facing
			*
			* Used for player movement and view matrix
		**/
		const glm::vec3& GetFront() const { return _front; }

		/**
			* @brief Returns the ortho right direction to the front
			* @return the ortho right direction to the front
			*
			* Used for player movement
		**/
		const glm::vec3& GetRight() const { return _right; }

		/**
			* @brief Returns the player's local up direction
			* @return the player's local up direction
			*
			* Used for player movement and view matrix
		**/
		const glm::vec3& GetUp() const { return _up; }

		/**
			* @brief Computes the players front
			* @param [dx] the horizontal delta
			* @param [dy] the vertical delta
			*
			* To be used with mouse input
		**/
		void ComputeFront(float dx, float dy);

		/**
			* @brief Moves the player in the physics world
			* @param [direction] the direction to move in relative to the player's front
			*
			* Used for player movement
		**/
		void MovePlayer(Math::vec3& direction);

		/**
			* @brief Provides a falling motion for the player
			* @param [dt] the frametime passed
			*
			* To be used in the on update loop
		**/
		void ProcessFalling(float dt);

	private:
		/**
			* @brief Computes the vector for the front direction
			*
			* To be used with ComputeFront
		**/
		void UpdateVectors();

		/**
			* @brief Checks if player is in contact with the ground
			* @retval true if player is on ground
			* @retval false if player is NOT on the ground
			* 
			* Used to set the grounded and falling states
		**/
		bool IsGrounded();

		/**
			* @brief Projects the player's movement direction to be parallel with a surface
			* @param [direction] the movement direction
			* @param [normal] the surface normal
			*
			* Uses the 'collide and slide' technique for player motion
		**/
		Math::vec3 DirectionFromNormal(const Math::vec3& direction, const Math::vec3& normal);

		// the movement drag factor
		float _moveDrag;

		// the fall drag factor
		float _fallDrag;

		// the movement speed factor
		float _moveFactor;

		// the front displacement factor
		float _sensitivity;

		// the yaw of the front
		float _yaw;

		// the pitch of the front
		float _pitch;

		// the height of the player
		float _capsuleHeight;

		// the fraction of the ground check penetration
		float _hitFraction;

		// the state the player is in if they are grounded
		bool _inGroundedState;

		// the state the player is in if they are falling
		bool _inFallingState;

		// the speed of the fall
		float _fallingSpeed;

		// the player's front direction
		Math::vec3 _front;

		// the worl'd up direction
		Math::vec3 _worldUp;

		// the player's right direction
		Math::vec3 _right;

		// the player's local up direction
		Math::vec3 _up;

		// the player's current velocity
		Math::vec3 _currentVelocity;

		// the start of the ray cast
		Math::vec3 _rayStart;

		// the end of the ray cast
		Math::vec3 _rayEnd;

		// the Raycaster
		Raycaster* _raycaster;

		// the player's physics body
		RigidBody* _body;

		// the reference to the physics world
		PhysicsWorld& _world;

		// the player's current orientation
		Math::quat _currentOrientation;
	};
}



