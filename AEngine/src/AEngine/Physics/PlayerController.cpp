#include "PlayerController.h"

AEngine::PlayerController::PlayerController(PhysicsWorld& world, const Math::vec3& startPosition,
	float radius, float height, const Prop& prop) :
	_moveDrag(prop.MoveDrag), _fallDrag(prop.FallDrag), _moveFactor(prop.MoveFactor),
	_sensitivity(prop.Sensitivity), _yaw(0), _pitch(0), _capsuleHeight(0), _hitFraction(0), _inGroundedState(false),
	_inFallingState(true), _fallingSpeed(0), _front(Math::vec3(0)), _worldUp(prop.WorldUp),
	_right(Math::cross(_front, _worldUp)), _up(Math::cross(_right, _front)), _body(nullptr), _world(world),
	_currentVelocity(Math::vec3(0)), _rayStart(Math::vec3(0)), _rayEnd(Math::vec3(0)),
	_currentOrientation(Math::quat(0, 0, 0, 0)), _raycaster(nullptr)
{
	_body = dynamic_cast<RigidBody*>(_world.AddRigidBody(startPosition, Math::quat(0, 0, 0, 0)));
	_body->AddCapsuleCollider(radius, height);
	_raycaster = Raycaster::Create(_world);
}

const void AEngine::PlayerController::GetTransform(Math::vec3& position, Math::quat& orientation) const
{
	_body->GetTransform(position, orientation);
}

void AEngine::PlayerController::ComputeFront(float dx, float dy)
{
	_yaw = Math::mod(_yaw + dx * _sensitivity, 360.f);

	_pitch += dy * _sensitivity;
	if (_pitch > 89.9f) _pitch = 89.9f;
	else if (_pitch < -89.9f) _pitch = -89.9f;

	UpdateVectors();
}

void AEngine::PlayerController::MovePlayer(Math::vec3& direction)
{
	if (direction == glm::vec3(0))
		return;

	direction = glm::normalize(direction);

	direction *= _moveFactor;

	direction.y = _fallingSpeed;

	if (!_inFallingState)
		direction = DirectionFromNormal(direction, _raycaster->GetInfo().hitNormal);

	_body->SetVelocity(direction);
}

void AEngine::PlayerController::ProcessFalling(float dt)
{
	_currentVelocity = _body->GetVelocity();

	if (IsGrounded())
	{
		if (!_inGroundedState)
		{
			_body->SetDrag(_moveDrag);
			_body->SetVelocity({ _currentVelocity.x, 0, _currentVelocity.z });
			_fallingSpeed = 0;
			_inGroundedState = true;
			_inFallingState = false;

			float hitFraction = _raycaster->GetInfo().hitFraction;

			if (hitFraction < 1.f)
			{
				Math::vec3 position;
				Math::quat orientation;
				float correction = (1.f - hitFraction) * 0.5f * _capsuleHeight;

				_body->GetTransform(position, orientation);
				position.y += correction;
				_body->SetTransform(position, orientation);
			}
		}
	}
	else
	{
		if (!_inFallingState)
		{
			_body->SetDrag(_fallDrag);
			_inFallingState = true;
			_inGroundedState = false;
		}

		_currentVelocity.y = _fallingSpeed;
		_body->SetVelocity(_currentVelocity);
		_fallingSpeed += -9.8f * dt;
	}
}

void AEngine::PlayerController::UpdateVectors()
{
	_front.x = cos(Math::radians(_yaw)) * cos(Math::radians(_pitch));
	_front.y = sin(Math::radians(_pitch));
	_front.z = sin(Math::radians(_yaw)) * cos(Math::radians(_pitch));
	_front = Math::normalize(_front);
	_right = Math::normalize(Math::cross(_front, _worldUp));
	_up = Math::normalize(Math::cross(_right, _front));
}

bool AEngine::PlayerController::IsGrounded()
{
	_body->GetTransform(_rayStart, _currentOrientation);
	_rayEnd = _rayStart + 0.5f * _capsuleHeight * Math::vec3(0, -1.f, 0);
	
	return (_raycaster->CastRay(_rayStart, _rayEnd));
}

AEngine::Math::vec3 AEngine::PlayerController::DirectionFromNormal(const Math::vec3& direction, const Math::vec3& normal)
{
	float projectionFactor = Math::dot(direction, normal) / glm::length2(normal);

	return (direction - projectionFactor * normal);
}
