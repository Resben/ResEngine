#pragma once
#include "AEngine/Core/TimeStep.h"
#include "CollisionBody.h"
#include "AEngine/Math/Math.hpp"

namespace AEngine
{
	class PhysicsWorld
	{
	public:
		struct Props
		{
			TimeStep updateStep{ 1.0f / 60.0f };
		};

		virtual void Init(const Props& settings) = 0;
		virtual void OnUpdate(TimeStep deltaTime) = 0;
		virtual CollisionBody* AddCollisionBody(const Math::vec3& position, const Math::quat& orientation) = 0;
		virtual CollisionBody* AddRigidBody(const Math::vec3& position, const Math::quat& orientation) = 0;
	};

    class PhysicsAPI
    {
    public:
		static PhysicsAPI& Instance();
        virtual PhysicsWorld* CreateWorld(const PhysicsWorld::Props& props = PhysicsWorld::Props()) = 0;
        virtual void DestroyWorld(PhysicsWorld* world) = 0;
		
    protected:
        PhysicsAPI() = default;
    };
}
