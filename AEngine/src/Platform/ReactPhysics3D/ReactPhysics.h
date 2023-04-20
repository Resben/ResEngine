#pragma once
#include <reactphysics3d/reactphysics3d.h>
#include "AEngine/Physics/CollisionBody.h"
#include "AEngine/Physics/Physics.h"
#include "AEngine/Core/TimeStep.h"
#include "AEngine/Math/Math.hpp"

namespace AEngine
{
    rp3d::Vector3 AEMathToRP3D(const Math::vec3& vec);

    rp3d::Quaternion AEMathToRP3D(const Math::quat& quat);

    Math::vec3 RP3DToAEMath(const rp3d::Vector3& vec);

    Math::quat RP3DToAEMath(const rp3d::Quaternion quat);

    class ReactPhysicsAPI : public PhysicsAPI
    {
    public:
        static ReactPhysicsAPI& Instance();
        virtual PhysicsWorld* CreateWorld(const PhysicsWorld::Props& props);
        virtual void DestroyWorld(PhysicsWorld* world);

        rp3d::PhysicsCommon* GetCommon();

    private:
        ReactPhysicsAPI();
        rp3d::PhysicsCommon m_common;
    };

    class ReactPhysicsWorld : public PhysicsWorld
    {
    public:
        ReactPhysicsWorld(rp3d::PhysicsCommon* common);
        virtual void Init(const Props& settings = Props()) override;
        virtual void OnUpdate(TimeStep deltaTime) override;
        virtual CollisionBody* AddCollisionBody(const Math::vec3& position, const Math::quat& orientation) override;
        virtual CollisionBody* AddRigidBody(const Math::vec3& position, const Math::quat& orientation) override;

		rp3d::PhysicsWorld* GetNative();

    private:
        rp3d::PhysicsWorld* m_world;
        float m_accumulator;
        float m_updateStep;
    };
}
