/**
 * \file   ReactRaycaster.h
 * \author Lane O'Rafferty (33534304)
*/

#pragma once
#include "AEngine/Physics/Raycaster.h"
#include <reactphysics3d/reactphysics3d.h>

namespace AEngine
{
		/**
		 * \class ReactRaycaster
		 * \brief Represents a raycaster in ReactPhysics.
		 *
		 * This class inherits from Raycaster and implements the rp3d::RaycastCallback interface.
		 */
	class ReactRaycaster : public Raycaster, public rp3d::RaycastCallback
	{
	public:
			/**
			 * \brief Constructor for ReactRaycaster.
			 *
			 * \param[in] world The PhysicsWorld associated with the raycaster.
			 */
		ReactRaycaster(PhysicsWorld* world);
			/**
			 * \brief Callback function called when a raycast hit occurs.
			 *
			 * \param[in] info The information about the raycast hit.
			 * \return The hit fraction.
			 */
		virtual rp3d::decimal notifyRaycastHit(const rp3d::RaycastInfo& info) override;
			/**
			 * \brief Casts a ray from the specified start to end position.
			 *
			 * \param[in] rayStart The starting position of the ray.
			 * \param[in] rayEnd The ending position of the ray.
			 * \return True if the ray hits any objects, false otherwise.
			 */
		virtual bool CastRay(Math::vec3 rayStart, Math::vec3 rayEnd) override;
			/**
			 * \brief Gets the information about the last raycast hit.
			 *
			 * \return The RayCastInfo of the last hit.
			 */
		virtual const RayCastInfo& GetInfo() const override;

	private:
		PhysicsWorld* _world; 	///< The associated PhysicsWorld.
		RayCastInfo _info; 		///< The information about the last raycast hit.
		bool _isHit; 			///< Flag indicating whether a hit occurred.
	};
}
