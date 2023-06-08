/**
 * \file   ReactRaycaster.cpp
 * \author Lane O'Rafferty (33534304)
*/

#include "ReactRaycaster.h"
#include "Platform/ReactPhysics3D/ReactPhysics.h"

namespace AEngine
{
    ReactRaycaster::ReactRaycaster(PhysicsWorld* world) :
        _world(world), _isHit(false)
    {

    }

    rp3d::decimal AEngine::ReactRaycaster::notifyRaycastHit(const rp3d::RaycastInfo& info)
    {
        _isHit = true;
        _info.hitFraction = info.hitFraction;
        _info.hitNormal = RP3DToAEMath(info.worldNormal);
        _info.hitPoint = RP3DToAEMath(info.worldPoint);

        return rp3d::decimal(0);
    }

    bool AEngine::ReactRaycaster::CastRay(Math::vec3 rayStart, Math::vec3 rayEnd)
    {
        _isHit = false;

        dynamic_cast<ReactPhysicsWorld*>(_world)->GetNative()->raycast({AEMathToRP3D(rayStart), AEMathToRP3D(rayEnd)}, this);

        return _isHit;
    }

    const RayCastInfo& AEngine::ReactRaycaster::GetInfo() const
    {
        return _info;
    }
}
