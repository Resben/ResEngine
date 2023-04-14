/**
 * @file
 * @author Christien Alden (34119981)
 * @brief Perspective camera interface
**/
#pragma once
#include "../Math/Math.hpp"

namespace AEngine
{
		/**
		 * @class PerspectiveCamera
		 * @brief Abstraction of a simple perspective camera
		**/
	class PerspectiveCamera
	{
	public:
			/**
			 * @brief Create PerspectiveCamera with default settings
			**/
		PerspectiveCamera();
			/**
			 * @brief Create PerspectiveCamera with user-defined settings
			 * @param[in] fov vertical field of view in degrees
			 * @param[in] aspect ratio (width / height)
			 * @param[in] nearPlane near clip distance
			 * @param[in] farPlane far clip distance
			**/
		PerspectiveCamera(float fov, float aspect, float nearPlane, float farPlane);
		virtual ~PerspectiveCamera() = default;

			/**
			 * @brief Sets a new field of view
			 * @param[in] fov field of view to apply in degrees
			 * @return void
			**/
		void SetFov(float fov);
			/**
			 * @brief Returns field of view in degrees
			 * @return float
			**/
		float GetFov() const;

			/**
			 * @brief Sets a new aspect ratio
			 * @param[in] aspect ratio to apply
			 * @return void
			**/
		void SetAspect(float aspect);
			/**
			 * @brief Returns aspect ratio
			 * @return float
			**/
		float GetAspect() const;

			/**
			 * @brief Sets a new near clip distance
			 * @param[in] nearPlane distance to apply
			 * @return void
			**/
		void SetNearPlane(float nearPlane);
			/**
			 * @brief Returns near clip distance
			 * @return float
			**/
		float GetNearPlane() const;

			/**
			 * @brief Sets a new far clip distance
			 * @param[in] farPlane distance to apply
			 * @return void
			**/
		void SetFarPlane(float farPlane);
			/**
			 * @brief Returns far clip distance
			 * @return float
			**/
		float GetFarPlane() const;

			/**
			 * @brief Sets the cameras view matrix
			 * @param[in] view matrix to apply
			 * @return void
			**/
		void SetViewMatrix(const Math::mat4& view);

			/**
			 * @brief Returns the current view matrix
			 * @return const Math::mat4&
			**/
		const Math::mat4& GetViewMatrix() const;
			/**
			 * @brief Returns the current projection matrix
			 * @return const Math::mat4&
			**/
		const Math::mat4& GetProjectionMatrix() const;
			/**
			 * @brief Returns a combined projection and view matrix
			 * @return projectionMat * viewMat
			**/
		const Math::mat4 GetProjectionViewMatrix() const;

	protected:
		Math::mat4 m_projection{1.0f};
		Math::mat4 m_view{1.0f};
		float m_fov = 45.0f;
		float m_aspect = (float) 1600 / 900;
		float m_nearPlane = 1.0f;
		float m_farPlane = 250.0f;

			/**
			 * @brief Recalculates the projection matrix
			 * @return void
			**/
		void CalculateProjectionMatrix();
	};
}
