#include "DebugCamera.h"
#include "AEngine/Core/Input.h"
#include "AEngine/Core/Application.h"
#include "AEngine/Core/Logger.h"

namespace AEngine
{
	DebugCamera::DebugCamera(float fov, float aspect, float nearPlane, float farPlane)
		: PerspectiveCamera(fov, aspect, nearPlane, farPlane), m_step{}, m_sensitivity{}, m_pitch{}, m_yaw{}
	{

	}

	void DebugCamera::OnUpdate(TimeStep frameTime)
	{
		UpdatePosition(frameTime);
		UpdateOrientation();
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
		Math::vec2 offset = Application::Instance().Input().GetMouseDelta();
		// update pitch / yaw
		m_pitch -= offset.y * m_sensitivity;
		m_yaw += offset.x * m_sensitivity;

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
		InputQuery& in = Application::Instance().Input();
		float offset = m_step * dt;

		// forward/back
		if (in.IsKeyPressed(AEKey::W))
			m_pos += offset * m_front;
		if (in.IsKeyPressed(AEKey::S))
			m_pos -= offset * m_front;

		// strafe
		if (in.IsKeyPressed(AEKey::A))
			m_pos -= offset * m_right;
		if (in.IsKeyPressed(AEKey::D))
			m_pos += offset * m_right;

		// up/down
		if (in.IsKeyPressed(AEKey::SPACE))
			m_pos += offset * m_up;
		if (in.IsKeyPressed(AEKey::LEFT_SHIFT))
			m_pos -= offset * m_up;
	}

	inline void DebugCamera::GenerateViewMatrix()
	{
		m_view = Math::lookAt(m_pos, m_pos + m_front, m_up);
	}

	void DebugCamera::SetLookSensitivity(float sensitivity)
	{
		m_sensitivity = sensitivity;
		if (m_sensitivity < 0.0f) { m_sensitivity = 0.0f; }
	}

	float DebugCamera::GetLookSensitivity() const
	{
		return m_sensitivity;
	}

	void DebugCamera::SetMovementStep(float step)
	{
		m_step = step;
		if (m_step < 0.0f) { m_step = 0.0f; }
	}

	float DebugCamera::GetMovementStep() const
	{
		return m_step;
	}
}
