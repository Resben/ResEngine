#include "Physics.h"

#ifdef AE_PHYSICS_REACT
	#include "Platform/ReactPhysics3D/ReactPhysics.h"
#else
	#error "No physics API defined"
#endif

namespace AEngine
{
	PhysicsAPI& PhysicsAPI::Instance()
	{
#ifdef AE_PHYSICS_REACT
		return ReactPhysicsAPI::Instance();
#endif
	}
}
