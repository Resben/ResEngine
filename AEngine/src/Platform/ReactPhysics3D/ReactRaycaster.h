#pragma once

#include "AEngine/Physics/Raycaster.h"
#include <reactphysics3d/reactphysics3d.h>

namespace AEngine
{
	/**
		* @class ReactRaycaster
		* @brief Implementation for the Raycaster class
		* @author Lane O'Rafferty (33534304)
	**/
	class ReactRaycaster : public Raycaster, public rp3d::RaycastCallback
	{
	public:
		ReactRaycaster(PhysicsWorld* world);

		virtual rp3d::decimal notifyRaycastHit(const rp3d::RaycastInfo& info) override;

		virtual bool CastRay(Math::vec3 rayStart, Math::vec3 rayEnd) override;

		virtual const RayCastInfo& GetInfo() const override;

	private:

		// the physics world
		PhysicsWorld* _world;

		// the info from the raycast hit
		RayCastInfo _info;

		// the hit success of the raycast
		bool _isHit;
	};
}

