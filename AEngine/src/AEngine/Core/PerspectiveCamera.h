/**
 * \file
 * \author Christien Alden (34119981)
*/
#pragma once
#include "AEngine/Math/Math.h"

namespace AEngine
{
		/**
		 * \class PerspectiveCamera
		 * \brief Abstraction of a simple perspective camera
		*/
	class PerspectiveCamera
	{
	public:
			/**
			 * \details
			 * Sets the following default values:
			 * - fov = 45 degrees
			 * - aspect = 16:9
			 * - near clip = 1.0f
			 * - far clip = 250.0f
			*/
		PerspectiveCamera();
			/**
			 * \param[in] fov vertical field of view in degrees
			 * \param[in] aspect ratio (width / height)
			 * \param[in] nearPlane near clip distance
			 * \param[in] farPlane far clip distance
			*/
		PerspectiveCamera(float fov, float aspect, float nearPlane, float farPlane);
		virtual ~PerspectiveCamera() = default;

			/**
			 * \brief Sets a new field of view
			 * \param[in] fov field of view to apply in degrees
			*/
		void SetFov(float fov);
			/**
			 * \brief Sets a new aspect ratio
			 * \param[in] aspect ratio to apply
			*/
		void SetAspect(float aspect);
			/**
			 * \brief Sets a new near clip distance
			 * \param[in] nearPlane distance to apply
			*/
		void SetNearPlane(float nearPlane);
			/**
			 * \brief Sets a new far clip distance
			 * \param[in] farPlane distance to apply
			*/
		void SetFarPlane(float farPlane);
			/**
			 * \brief Sets the cameras view matrix
			 * \param[in] viewMatrix to apply
			*/
		void SetViewMatrix(const Math::mat4& viewMatrix);

			/**
			 * \brief Returns field of view in degrees
			 * \return Field of view in degrees
			*/
		float GetFov() const;
			/**
			 * \brief Returns aspect ratio
			 * \return Aspect ratio
			*/
		float GetAspect() const;
			/**
			 * \brief Returns near clip distance
			 * \return Near clip distance
			*/
		float GetNearPlane() const;
			/**
			 * \brief Returns far clip distance
			 * \return Far clip distance
			*/
		float GetFarPlane() const;
			/**
			 * \brief Returns the current view matrix
			 * \return View matrix of camera
			*/
		const Math::mat4& GetViewMatrix() const;
			/**
			 * \brief Returns the current projection matrix
			 * \return Projection matrix of camera
			*/
		const Math::mat4& GetProjectionMatrix() const;
			/**
			 * \brief Returns a combined projection and view matrix
			 * \return The combined projection and view matrix
			 * \details
			 * This is achieved by multiplying the projection matrix by the view matrix:
			 * PVM = P * V
			*/
		const Math::mat4 GetProjectionViewMatrix() const;

	protected:
		Math::mat4 m_projection;
		Math::mat4 m_view;
		float m_fov;
		float m_aspect;
		float m_nearPlane;
		float m_farPlane;

			/**
			 * \brief Recalculates the projection matrix
			*/
		void CalculateProjectionMatrix();
	};
}
