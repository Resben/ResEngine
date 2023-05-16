#include "DebugCamera.h"
#include "AEngine/Core/Input.h"
#include "AEngine/Core/Application.h"
#include "AEngine/Core/Logger.h"

namespace AEngine
{
	DebugCamera::DebugCamera(float fov, float aspect, float nearPlane, float farPlane)
		: PerspectiveCamera(fov, aspect, nearPlane, farPlane), m_moveSpeed{}, m_lookSensitivity{}, m_pitch{}, m_yaw{}
	{

	}

	void DebugCamera::OnUpdate(TimeStep frameTime)
	{
		UpdateOrientation();
		UpdatePosition(frameTime);
		GenerateViewMatrix();
	}

	Math::vec3 DebugCamera::GetPosition() const
	{
		return m_pos;
	}

	void DebugCamera::SetYaw(float yaw)
	{
		m_yaw = yaw;
	}

	inline void DebugCamera::UpdateOrientation()
	{
		float lookStep = m_lookSensitivity * m_lookFactor;
		Math::vec2 offset = Input::GetMouseDelta();
		// update pitch / yaw
		m_pitch -=  lookStep * offset.y;
		m_yaw += lookStep * offset.x;

		// clamp pitch
		if (m_pitch >= 89.0f) { m_pitch = 89.0f; }
		if (m_pitch <= -89.0f) { m_pitch = -89.0f; }

		// clamp yaw
		if (m_yaw >= 360.0f) { m_yaw = 0.0f; }
		if (m_yaw <= -360.0f) { m_yaw = 0.0f; }

		// update member variables
		Math::vec3 front;
		float pitchRad = Math::radians(m_pitch);
		float yawRad = Math::radians(m_yaw);
		front.x = Math::cos(yawRad) * Math::cos(pitchRad);
		front.y = Math::sin(pitchRad);
		front.z = Math::sin(yawRad) * Math::cos(pitchRad);
		m_right = Math::normalize(Math::cross(front, m_up));
		m_front = Math::normalize(front);
	}

	inline void DebugCamera::UpdatePosition(float dt)
	{
		float movementStep = (m_moveSpeed * m_moveFactor) * dt;

		// forward/back
		if (Input::IsKeyPressed(AEKey::W))
			m_pos += movementStep * m_front;
		if (Input::IsKeyPressed(AEKey::S))
			m_pos -= movementStep * m_front;

		// strafe
		if (Input::IsKeyPressed(AEKey::A))
			m_pos -= movementStep * m_right;
		if (Input::IsKeyPressed(AEKey::D))
			m_pos += movementStep * m_right;

		// up/down
		if (Input::IsKeyPressed(AEKey::SPACE))
			m_pos += movementStep * m_up;
		if (Input::IsKeyPressed(AEKey::LEFT_SHIFT))
			m_pos -= movementStep * m_up;
	}

	inline void DebugCamera::GenerateViewMatrix()
	{
		m_view = Math::lookAt(m_pos, m_pos + m_front, m_up);
	}

	void DebugCamera::SetLookSensitivity(float sensitivity)
	{
		m_lookSensitivity = sensitivity;
		if (m_lookSensitivity < 0.0f) { m_lookSensitivity = 0.0f; }
	}

	float DebugCamera::GetLookSensitivity() const
	{
		return m_lookSensitivity;
	}

	void DebugCamera::SetMovementStep(float speed)
	{
		m_moveSpeed = speed;
		if (m_moveSpeed < 0.0f) { m_moveSpeed = 0.0f; }
	}

	float DebugCamera::GetMovementStep() const
	{
		return m_moveSpeed;
	}
}
