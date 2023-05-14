/**
 * @file
 * @author Christien Alden (34119981)
**/
#pragma once
#include "AEngine/Core/PerspectiveCamera.h"
#include "AEngine/Core/TimeStep.h"

namespace AEngine
{
		/**
		 * @class DebugCamera
		 * @brief PerspectiveCamera used to explore the scene without interaction
		**/
	class DebugCamera : public PerspectiveCamera
	{
	public:
		DebugCamera() = default;
		DebugCamera(float fov, float aspect, float nearPlane, float farPlane);

			/**
			 * @brief Polls input and updates camera orientation and position
			 * @return void
			**/
		void OnUpdate(TimeStep frameTime);

		Math::vec3 GetPosition() const;
		void SetYaw(float yaw);

			/**
			 * @brief Sets the mouse look sentitivity
			 * @param[in] sensitivity to set
			 * @retval void
			**/
		void SetLookSensitivity(float sensitivity);
			/**
			 * @brief Returns the current sentitivity
			 * @retval float Sensitivity of camera
			**/
		float GetLookSensitivity() const;

			/**
			 * @brief Sets the move step
			 * @param[in] step Movement step of camera per second
			 * @retval void
			**/
		void SetMovementStep(float step);
			/**
			 * @brief Returns the current movement step
			 * @retval float Movement step of camera
			**/
		float GetMovementStep() const;

	private:
		Math::vec3 m_pos{ 0.0f, 0.0f, 0.0f };	///< Camera position
		Math::vec3 m_front{ 1.0f, 0.0f, 0.0f };	///< Camera front vector
		Math::vec3 m_up{ 0.0f, 1.0f, 0.0f };	///< Camera up vector
		Math::vec3 m_right{ 0.0f, 0.0f, 1.0f };	///< Camera right vector

		float m_step{ 10.0f };					///< Translation step
		float m_sensitivity{ 0.07f };			///< Rotation sensitivity
		float m_pitch{ 0.0f };					///< Pitch offset
		float m_yaw{ 90.0f };					///< Yaw offset, face z-direction

			/**
			 * @brief Queries Input for cursor movement to update camera orientation
			 * @retval void
			**/
		void UpdateOrientation();
			/**
			 * @brief Queries Input for keypresses to update camera position
			 * @retval void
			**/
		void UpdatePosition(float dt);
			/**
			 * @brief Generates view matrix based on member variable values
			 * @retval void
			**/
		void GenerateViewMatrix();
	};
}
