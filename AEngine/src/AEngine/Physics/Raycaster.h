/**
 * \file
 * \author Lane O'Rafferty (33534304)
*/
#pragma once
#include "AEngine/Physics/Physics.h"

namespace AEngine
{
		/**
		 * \struct RayCastInfo
		 * \brief Stores information about a raycast
		*/
	struct RayCastInfo
	{
		float hitFraction = 0;
		Math::vec3 hitPoint = { 0, 0, 0 };
		Math::vec3 hitNormal = { 0, 0, 0 };
	};

		/**
		 * \class Raycaster
		 * \brief Raycaster Class to be used by the physics system
		 * \author Lane O'Rafferty (33534304)
		*/
	class Raycaster
	{
	public:
		Raycaster() = default;
		virtual ~Raycaster() = default;

			/**
			 * \brief Casts a ray and gets info if there was a hit
			 * \param[in] rayStart the origin point of the ray
			 * \param[in] rayEnd the end point of the ray
			 * \retval true if ray hit a collider
			 * \retval false if ray did not hit a collider
			*/
		virtual bool CastRay(Math::vec3 rayStart, Math::vec3 rayEnd) = 0;
			/**
			 * \brief Returns the info stored by the raycaster
			 * \return The ray hit info
			*/
		virtual const RayCastInfo& GetInfo() const = 0;
			/**
			 * \brief Factory method to create a raycaster
			 * \return Pointer to a RayCaster object
			*/
		static Raycaster* Create(PhysicsWorld& world);
	};
}