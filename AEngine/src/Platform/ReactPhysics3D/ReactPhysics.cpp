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

	Math::vec3 RP3DToAEMath(const rp3d::Vector3& vec)
	{
		return { vec.x, vec.y, vec.z };
	}

	rp3d::Quaternion AEMathToRP3D(const Math::quat& quat)
	{
		return { quat.x, quat.y, quat.z, quat.w };
	}

	Math::quat RP3DToAEMath(const rp3d::Quaternion quat)
	{
		return { quat.w,  quat.x, quat.y, quat.z };
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
		: m_world(nullptr), m_accumulator(0), m_updateStep(0), m_renderer{ nullptr }
	{
		m_world = common->createPhysicsWorld();
		m_renderer = MakeUnique<ReactPhysicsRenderer>(m_world->getDebugRenderer());
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
			m_world->update(m_updateStep.Seconds());
			m_accumulator -= m_updateStep;
		}
	}

	CollisionBody* ReactPhysicsWorld::AddCollisionBody(const Math::vec3& position, const Math::quat& orientation)
	{
		return new ReactCollisionBody(this, position, orientation);
	}

	RigidBody* ReactPhysicsWorld::AddRigidBody(const Math::vec3& position, const Math::quat& orientation)
	{
		return new ReactRigidBody(this, position, orientation);
	}

	void ReactPhysicsWorld::Render(const Math::mat4& projectionView) const
	{
		m_renderer->Render(projectionView);
	}

	bool ReactPhysicsWorld::IsRenderingEnabled() const
	{
		return m_world->getIsDebugRenderingEnabled();
	}

	void ReactPhysicsWorld::SetRenderingEnabled(bool enable)
	{
		m_world->setIsDebugRenderingEnabled(enable);
	}

	const ReactPhysicsRenderer* ReactPhysicsWorld::GetRenderer() const
	{
		return m_renderer.get();
	}

	rp3d::PhysicsWorld* ReactPhysicsWorld::GetNative()
	{
		return m_world;
	}
}
