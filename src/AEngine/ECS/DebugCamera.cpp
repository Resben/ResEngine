#include "AEngine/ECS/DebugCamera.h"
#include "AEngine/Core/InputManager.h"

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

	glm::vec3 DebugCamera::GetPosition() const
	{
		return m_pos;
	}

	inline void DebugCamera::UpdateOrientation()
	{
		static bool first = true;
		glm::vec2 cursorOffset = InputManager::GetMouseDelta();

		if (!first)
		{
			// update pitch / yaw
			m_yaw += cursorOffset.x * m_sensitivity;
			m_pitch -= cursorOffset.y * m_sensitivity;

			// clamp pitch
			if (m_pitch >= 89.0f) { m_pitch = 89.0f; }
			if (m_pitch <= -89.0f) { m_pitch = -89.0f; }

			// clamp yaw
			if (m_yaw >= 360.0f) { m_yaw = 0.0f; }
			if (m_yaw <= -360.0f) { m_yaw = 0.0f;  }
		}

		// update member variables
		glm::vec3 front;
		float pitchRad = glm::radians(m_pitch);
		float yawRad = glm::radians(m_yaw);
		front.x = glm::cos(yawRad) * glm::cos(pitchRad);
		front.y = glm::sin(pitchRad);
		front.z = glm::sin(yawRad) * glm::cos(pitchRad);
		m_front = glm::normalize(front);
		m_right = glm::cross(m_front, m_up);
		first = false;
	}

	inline void DebugCamera::UpdatePosition(float dt)
	{
		float offset = m_step * dt;

		if (InputManager::IsKeyPressed(GLFW_KEY_W))
			m_pos += offset * m_front;
		if (InputManager::IsKeyPressed(GLFW_KEY_S))
			m_pos -= offset * m_front;

		// strafe
		if (InputManager::IsKeyPressed(GLFW_KEY_A))
			m_pos -= offset * m_right;
		if (InputManager::IsKeyPressed(GLFW_KEY_D))
			m_pos += offset * m_right;

		// up/down
		if (InputManager::IsKeyPressed(GLFW_KEY_SPACE))
			m_pos += offset * m_up;
		if (InputManager::IsKeyPressed(GLFW_KEY_LEFT_SHIFT))
			m_pos -= offset * m_up;
	}

	inline void DebugCamera::GenerateViewMatrix()
	{
		m_view = glm::lookAt(m_pos, m_pos + m_front, m_up);
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
