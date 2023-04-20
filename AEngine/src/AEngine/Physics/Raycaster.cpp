#include "Raycaster.h"
#include "Platform/ReactPhysics3D/ReactRaycaster.h"

namespace AEngine
{
	Raycaster* Raycaster::Create(PhysicsWorld& world)
	{
#ifdef AE_PHYSICS_REACT
		return new ReactRaycaster(world);
#endif
	}
}

