/**
 * \file
 * \author Lane O'Rafferty (33534304)
*/
#include "Raycaster.h"

#ifdef AE_PHYSICS_REACT
	#include "Platform/ReactPhysics3D/ReactRaycaster.h"
#endif

namespace AEngine
{
	Raycaster* Raycaster::Create(PhysicsWorld* world)
	{
#ifdef AE_PHYSICS_REACT
		return new ReactRaycaster(world);
#else
	#error "No physics engine defined"
#endif
	}
}
