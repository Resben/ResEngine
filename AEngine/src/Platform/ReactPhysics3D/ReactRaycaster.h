#pragma once

#include "AEngine/Physics/Raycaster.h"
#include <reactphysics3d/reactphysics3d.h>

namespace AEngine
{
	class ReactRaycaster : public Raycaster, public rp3d::RaycastCallback
	{
	public:
		ReactRaycaster(PhysicsWorld& world);

		virtual rp3d::decimal notifyRaycastHit(const rp3d::RaycastInfo& info) override;

		virtual bool CastRay(Math::vec3 rayStart, Math::vec3 rayEnd) override;

		virtual const RayCastInfo& GetInfo() const override;

	private:
		PhysicsWorld& _world;

		RayCastInfo _info;

		bool _isHit;
	};
}

