/**
 * \file
 * \author Lane O'Rafferty (33534304)
*/
#include "PlayerController.h"

namespace AEngine
{
	PlayerController::PlayerController(
		PhysicsWorld* world,
		const Math::vec3& startPosition,
		const Properties& properties)
		: m_properties{ properties },
		m_capsuleHeight{ 0 },
		m_fallingSpeed{ 0 },
		m_currentVelocity{ Math::vec3{ 0.0f } },
		m_inGroundedState{ false },
		m_inFallingState{ true },
		m_body{ nullptr },
		m_raycaster{ nullptr }
	{
		m_body = world->AddRigidBody(startPosition, Math::quat(1, 0, 0, 0));
		m_body->AddCapsuleCollider(m_properties.radius, m_properties.height);
		m_raycaster = Raycaster::Create(world);
	}

	Math::vec3 PlayerController::GetTransform() const
	{
		Math::vec3 position;
		Math::quat orientation;
		m_body->GetTransform(position, orientation);
		return position;
	}

	void PlayerController::ApplyForce(Math::vec3& direction)
	{
		// check for zero vector
		if (Math::all(glm::equal(direction, Math::vec3(0))))
		{
			return;
		}

		// set the magnitude of the direction vector based off moveFactor and gravity
		direction = glm::normalize(direction);
		direction *= m_properties.moveFactor;
		direction.y = m_fallingSpeed;

		// if in falling state, do something
		/// \todo Lane, comment this!
		if (!m_inFallingState)
		{
			direction = DirectionFromNormal(direction, m_raycaster->GetInfo().hitNormal);
		}

		// sets the linear velocity of the rigidbody
		m_body->SetVelocity(direction);
	}

	void PlayerController::OnUpdate(float dt)
	{
		m_currentVelocity = m_body->GetVelocity();

		if (DetectState())
		{
			if (!m_inGroundedState)
			{
				m_body->SetDrag(m_properties.moveDrag);
				m_body->SetVelocity({ m_currentVelocity.x, 0, m_currentVelocity.z });
				m_fallingSpeed = 0;
				m_inGroundedState = true;
				m_inFallingState = false;

				float hitFraction = m_raycaster->GetInfo().hitFraction;

				if (hitFraction < 1.f)
				{
					Math::vec3 position;
					Math::quat orientation;
					float correction = (1.f - hitFraction) * 0.5f * m_capsuleHeight;

					m_body->GetTransform(position, orientation);
					position.y += correction;
					m_body->SetTransform(position, orientation);
				}
			}
		}
		else
		{
			if (!m_inFallingState)
			{
				m_body->SetDrag(m_properties.fallDrag);
				m_inFallingState = true;
				m_inGroundedState = false;
			}

			m_currentVelocity.y = m_fallingSpeed;
			m_body->SetVelocity(m_currentVelocity);
			m_fallingSpeed += -9.8f * dt;
		}
	}

	bool PlayerController::DetectState()
	{
		Math::vec3 rayStart;
		Math::quat orientation;
		m_body->GetTransform(rayStart, orientation);
		Math::vec3 rayEnd = rayStart + 0.5f * m_properties.height * Math::vec3(0, -1.0f, 0);
		return (m_raycaster->CastRay(rayStart, rayEnd));
	}

	Math::vec3 PlayerController::DirectionFromNormal(const Math::vec3& direction, const Math::vec3& normal)
	{
		float projectionFactor = Math::dot(direction, normal) / glm::length2(normal);
		return (direction - projectionFactor * normal);
	}
}
