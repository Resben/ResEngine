/**
 * \file
 * \author Lane O'Rafferty (33534304)
*/
#include "PlayerController.h"
#include <iostream>

namespace AEngine
{
	PlayerController::PlayerController(
		PhysicsWorld* world,
		const Math::vec3& startPosition,
		const Properties& properties)
		: m_properties{ properties },
		m_capsuleHeight{ 0 },
		m_fallingSpeed{ 0 },
		m_currentDirection{ Math::vec3{0.0f} },
		m_unitDirection{ Math::vec3{0.0f} },
		m_currentVelocity{ Math::vec3{ 0.0f } },
		m_inGroundedState{ false },
		m_inFallingState{ true },
		m_body{ nullptr },
		m_groundRay{ nullptr },
		m_forwardRay{ nullptr }
	{
		m_body = world->AddRigidBody(startPosition, Math::quat(1, 0, 0, 0));
		m_body->AddCapsuleCollider(m_properties.radius, m_properties.height);
		m_groundRay = Raycaster::Create(world);
		m_forwardRay = Raycaster::Create(world);
	}

	Math::vec3 PlayerController::GetTransform() const
	{
		Math::vec3 position;
		Math::quat orientation;
		m_body->GetInterpolatedTransform(position, orientation);
		return position;
	}

	void PlayerController::ApplyForce(Math::vec3& direction)
	{
		// check for zero vector
		if (Math::all(glm::equal(direction, Math::vec3(0))))
		{
			return;
		}

		// remove the y component from the direction (lock to ground)
		direction.y = 0.0f;

		// set the magnitude of the direction vector based off moveFactor and gravity
		m_unitDirection = glm::normalize(direction);

		// set the current direction off the computed unitDirection scaled by the move factor
		m_currentDirection = m_unitDirection * m_properties.moveFactor;

		// if the player hit a wall, project their movement parallel to the collision surface
		if (m_hitWall)
		{
			MoveAlongNormal(m_forwardRay->GetInfo().hitNormal);
		}

		// add the falling speed to the y component
		m_currentDirection.y += m_fallingSpeed;

		// applies movement on the body based on the current direction
		m_body->SetVelocity(m_currentDirection);
	}

	void PlayerController::OnUpdate(float dt)
	{
		if (DetectGround())
		{
			if (!m_inGroundedState)
			{
				std::cout << "Grounded" << std::endl;
				m_body->SetDrag(m_properties.moveDrag);
				m_body->SetVelocity({ m_currentDirection.x, 0, m_currentDirection.z });
				m_fallingSpeed = 0.0f;
				m_inGroundedState = true;
				m_inFallingState = false;

				float hitFraction = m_groundRay->GetInfo().hitFraction;

				if (hitFraction < 1.0f)
				{
					Math::vec3 position;
					Math::quat orientation;
					m_body->GetTransform(position, orientation);

					float correction = 1.0f - hitFraction;
					position.y += correction * 0.5f * m_properties.height;
					m_body->SetTransform(position, orientation);
				}
			}
		}
		else
		{
			if (!m_inFallingState)
			{
				std::cout << "Falling" << std::endl;
				m_body->SetDrag(m_properties.fallDrag);
				m_inFallingState = true;
				m_inGroundedState = false;
			}

			m_currentDirection.y = m_fallingSpeed;
			m_body->SetVelocity(m_currentDirection);
			m_fallingSpeed += -9.8f * dt;
		}

		if (DetectWall())
		{
			if (!m_hitWall)
			{
				m_body->SetVelocity({0.0f, m_fallingSpeed, 0.0f});
				m_hitWall = true;
			}
		}
		else
		{
			if (m_hitWall)
			{
				m_hitWall = false;
			}
		}
	}

	bool PlayerController::DetectGround()
	{
		Math::vec3 rayStart;
		Math::quat orientation;
		m_body->GetTransform(rayStart, orientation);
		Math::vec3 rayEnd = rayStart + 0.5f * m_properties.height * Math::vec3(0, -1.0f, 0);
		return (m_groundRay->CastRay(rayStart, rayEnd));
	}

	bool PlayerController::DetectWall()
	{
		Math::vec3 rayStart;
		Math::quat orientation;
		m_body->GetTransform(rayStart, orientation);
		rayStart.y -= 0.9f * 0.5f * m_properties.height;
		Math::vec3 rayEnd = rayStart + m_properties.radius * m_unitDirection;
		return (m_forwardRay->CastRay(rayStart, rayEnd));
	}

	void PlayerController::MoveAlongNormal(const Math::vec3& normal)
	{
		float factor = Math::dot(m_currentDirection, normal) / glm::length2(normal);
		m_currentDirection -= factor * normal;
	}
}
