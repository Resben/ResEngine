#include "AEngine/Core/TimeStep.h"
#include "ReactPhysics.h"
#include "ReactCollisionBody.h"

namespace AEngine
{
//--------------------------------------------------------------------------------
// Math Conversions
//--------------------------------------------------------------------------------
    rp3d::Vector3 AEMathToRP3D(const Math::vec3& vec)
    {
        return { vec.x, vec.y, vec.z };
    }

    rp3d::Quaternion AEMathToRP3D(const Math::quat& quat)
    {
        return { quat.x, quat.y, quat.z, quat.w };
    }

    Math::vec3 RP3DToAEMath(const rp3d::Vector3& vec)
    {
        return { vec.x, vec.y, vec.z };
    }

    Math::quat RP3DToAEMath(const rp3d::Quaternion quat)
    {
        return { quat.x, quat.y, quat.z, quat.w };
    }

//--------------------------------------------------------------------------------
// ReactPhysicsAPI
//--------------------------------------------------------------------------------
    ReactPhysicsAPI& ReactPhysicsAPI::Instance()
    {
        static ReactPhysicsAPI s_instance;
        return s_instance;
    }

    ReactPhysicsAPI::ReactPhysicsAPI()
        : m_common()
    {

    }

	PhysicsWorld* ReactPhysicsAPI::CreateWorld(const PhysicsWorld::Props& props)
    {
		PhysicsWorld* world = new ReactPhysicsWorld(&m_common);
        world->Init(props);
        return world;
    }

    void ReactPhysicsAPI::DestroyWorld(PhysicsWorld* world)
    {
        m_common.destroyPhysicsWorld(dynamic_cast<ReactPhysicsWorld*>(world)->GetNative());
    }

    rp3d::PhysicsCommon* ReactPhysicsAPI::GetCommon()
    {
        return &m_common;
    }

//--------------------------------------------------------------------------------
// ReactPhysicsWorld
//--------------------------------------------------------------------------------
    ReactPhysicsWorld::ReactPhysicsWorld(rp3d::PhysicsCommon* common)
        : m_world(nullptr), m_accumulator(0), m_updateStep(0)
    {
        m_world = common->createPhysicsWorld();
    }

    void ReactPhysicsWorld::Init(const Props& settings)
    {
		m_updateStep = settings.updateStep;
    }

    void ReactPhysicsWorld::OnUpdate(TimeStep deltaTime)
    {
        m_accumulator += deltaTime;
        
        while (m_accumulator >= m_updateStep)
        {
            m_world->update(m_updateStep);
            m_accumulator -= m_updateStep;
        }
    }
    
    CollisionBody* ReactPhysicsWorld::AddCollisionBody(const Math::vec3& position, const Math::quat& orientation)
    {
        return new ReactCollisionBody(m_world, position, orientation);
    }

    CollisionBody* ReactPhysicsWorld::AddRigidBody(const Math::vec3& position, const Math::quat& orientation)
    {
        return new ReactRigidBody(m_world, position, orientation);
    }

	rp3d::PhysicsWorld* ReactPhysicsWorld::GetNative()
	{
		return m_world;
	}
}
