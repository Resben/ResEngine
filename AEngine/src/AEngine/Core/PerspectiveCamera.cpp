/**
 * @file
 * @author Christien Alden (34119981)
 * @brief Pespective camera implementation
**/
#include "PerspectiveCamera.h"

namespace AEngine
{
	PerspectiveCamera::PerspectiveCamera()
	{
		CalculateProjectionMatrix();
	}

	PerspectiveCamera::PerspectiveCamera(float fov, float aspect, float nearFlane, float farPlane)
		: m_fov(fov), m_aspect(aspect), m_nearPlane(nearFlane), m_farPlane(farPlane)
	{
		CalculateProjectionMatrix();
	}

	const Math::mat4& PerspectiveCamera::GetViewMatrix() const
	{
		return m_view;
	}

	const Math::mat4& PerspectiveCamera::GetProjectionMatrix() const
	{
		return m_projection;
	}

	const Math::mat4 PerspectiveCamera::GetProjectionViewMatrix() const
	{
		return m_projection * m_view;
	}

	//--------------------------------------------------------------------------------
	// Projection Matrix Components
	//--------------------------------------------------------------------------------
	void PerspectiveCamera::SetFov(float fov)
	{
		m_fov = fov;
		CalculateProjectionMatrix();
	}

	float PerspectiveCamera::GetFov() const
	{
		return m_fov;
	}

	void PerspectiveCamera::SetAspect(float aspect)
	{
		m_aspect = aspect;
		CalculateProjectionMatrix();
	}

	float PerspectiveCamera::GetAspect() const
	{
		return m_aspect;
	}

	void PerspectiveCamera::SetNearPlane(float nearPlane)
	{
		m_nearPlane = nearPlane;
		CalculateProjectionMatrix();
	}

	float PerspectiveCamera::GetNearPlane() const
	{
		return m_nearPlane;
	}

	void PerspectiveCamera::SetFarPlane(float farPlane)
	{
		m_farPlane = farPlane;
		CalculateProjectionMatrix();
	}

	float PerspectiveCamera::GetFarPlane() const
	{
		return m_farPlane;
	}

	//--------------------------------------------------------------------------------
	// View Matrix Components
	//--------------------------------------------------------------------------------
	void PerspectiveCamera::SetViewMatrix(const Math::mat4& view)
	{
		m_view = view;
	}

	//--------------------------------------------------------------------------------
	// Private
	//--------------------------------------------------------------------------------
	void PerspectiveCamera::CalculateProjectionMatrix()
	{
		m_projection = Math::perspective(Math::radians(m_fov), m_aspect, m_nearPlane, m_farPlane);
	}
}
